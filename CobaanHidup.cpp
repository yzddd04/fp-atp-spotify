#include <iostream>
#include <string>
#include <algorithm>
#include <fstream> 

using namespace std;

struct Song {
    string name;
    string artist;
    string genre;
    Song* next;
};

struct Playlist {
    string name;
    Song* songs;
    Playlist* next;
};

// create new node
Song* createSong(string name, string artist, string genre) {
    Song* newSong = new Song();
    newSong->name = name;
    newSong->artist = artist;
    newSong->genre = genre;
    newSong->next = nullptr;
    return newSong;
}

// make new playlist
Playlist* createPlaylist(string name) {
    Playlist* newPlaylist = new Playlist();
    newPlaylist->name = name;
    newPlaylist->songs = nullptr;
    newPlaylist->next = nullptr;
    return newPlaylist;
}

// add song to playlist
bool addSongToPlaylist(Song*& playlistSongs, string name, string artist, string genre) {
    // cek jika lagu sudah ada, return false
    Song* temp = playlistSongs;
    while (temp != nullptr) {
        if (temp->name == name && temp->artist == artist) {
            cout << "Lagu '" << name << "' sudah ada di dalam playlist ini." << endl;
            return false;
        }
        temp = temp->next;
    }

    // jika lagu belum ada, tambahkan lagu baru dan return true
    Song* newSong = createSong(name, artist, genre);
    if (playlistSongs == nullptr) {
        playlistSongs = newSong;
    } else {
        temp = playlistSongs;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newSong;
    }
    cout << "Lagu '" << name << "' berhasil ditambahkan ke playlist." << endl; // hanya ditampilkan jika lagu berhasil ditambahkan
    return true; 
}



// menampilkan lagu dalam sebuah playlist
void displaySongs(Song* head) {
    if (head == nullptr) {
        cout << "Playlist kosong!" << endl;
        return;
    }
    Song* temp = head;
    int songIndex = 1;
    while (temp != nullptr) {
    cout << songIndex << ". " << temp->name << " - " << temp->artist << " (" << temp->genre << ")" << endl;
    temp = temp->next;
    songIndex++;
}

}

// menampilkan playlist yang ada
void displayPlaylists(Playlist* head) {
    if (head == nullptr) {
        cout << "Tidak ada playlist yang dibuat." << endl;
        return;
    }
    Playlist* temp = head;
    int index = 1;
    while (temp != nullptr) {
        cout << index << ". Playlist: " << temp->name << endl;
        temp = temp->next;
        index++;
    }
}

// inisialisasi lagu yang tersedia (by default)
void initializeAvailableSongs(Song*& availableSongs) {
    addSongToPlaylist(availableSongs, "Bohemian Rhapsody", "Queen", "Rock");
    addSongToPlaylist(availableSongs, "Rolling in the Deep", "Adele", "Pop");
    addSongToPlaylist(availableSongs, "Feeling Good", "Nina Simone", "Jazz");
    addSongToPlaylist(availableSongs, "Sweet Child O' Mine", "Guns N' Roses", "Rock");
    addSongToPlaylist(availableSongs, "Uptown Funk", "Mark Ronson ft. Bruno Mars", "Pop");
    addSongToPlaylist(availableSongs, "Take Five", "Dave Brubeck Quartet", "Jazz");
    addSongToPlaylist(availableSongs, "Hotel California", "Eagles", "Rock");
    addSongToPlaylist(availableSongs, "Blinding Lights", "The Weeknd", "Pop");
    addSongToPlaylist(availableSongs, "Fly Me to the Moon", "Frank Sinatra", "Jazz");
    addSongToPlaylist(availableSongs, "Imagine", "John Lennon", "Rock/Pop");
}

// convert lagu jadi array, buat sorting
int songListToArray(Song* head, Song* arr[]) {
    int count = 0;
    while (head != nullptr) {
        arr[count++] = head;
        head = head->next;
    }
    return count;
}

// sort by song name
void sortSongsByName(Song*& head) {
    if (head == nullptr || head->next == nullptr) return;

    Song* arr[100];
    int count = songListToArray(head, arr);

    sort(arr, arr + count, [](Song* a, Song* b) {
        return a->name < b->name;
    });

    // Rekonstruksi linked list
    head = arr[0];
    Song* temp = head;
    for (int i = 1; i < count; i++) {
        temp->next = arr[i];
        temp = temp->next;
    }
    temp->next = nullptr;
}

// sort by artist
void sortSongsByArtist(Song*& head) {
    if (head == nullptr || head->next == nullptr) return;

    Song* arr[100];
    int count = songListToArray(head, arr);

    sort(arr, arr + count, [](Song* a, Song* b) {
        return a->artist < b->artist;
    });

    // Rekonstruksi linked list
    head = arr[0];
    Song* temp = head;
    for (int i = 1; i < count; i++) {
        temp->next = arr[i];
        temp = temp->next;
    }
    temp->next = nullptr;
}

// delete song from playlist
void deleteSongFromPlaylist(Song*& head, string songName) {
    if (head == nullptr) { // jika playlist kosong
        cout << "Playlist kosong! Tidak ada lagu yang dapat dihapus." << endl;
        return;
    }

    // jika lagu yang ingin dihapus adalah head (delete from beginning)
    if (head->name == songName) {
        Song* temp = head;
        head = head->next;
        delete temp;
        cout << "Lagu '" << songName << "' berhasil dihapus dari playlist." << endl;
        return;
    }

    // cari lagunya
    Song* current = head;
    Song* previous = nullptr;

    while (current != nullptr && current->name != songName) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Lagu '" << songName << "' tidak ditemukan dalam playlist." << endl;
        return;
    }

    // hapus node lagu yang ditemukan
    previous->next = current->next;
    delete current;
    cout << "Lagu '" << songName << "' berhasil dihapus dari playlist." << endl;
}


// menambahkan lagu yg tersedia dari file txt
void readFromFile(Song*& head, const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Gagal membuka file: " << fileName << endl;
        return;
    }

    string line;
    int count = 0; // untuk menghitung jumlah lagu yg berhasil ditambahkan
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.rfind(',');
        // jika formmat tidak valdi
        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) {
            cout << "Format data tidak valid: " << line << endl;
            continue;
        }

        string name = line.substr(0, pos1);
        string artist = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string genre = line.substr(pos2 + 1);

        if (addSongToPlaylist(head, name, artist, genre)) {
            count++; // menghitung lagu yg berhasil ditambahkan
        }
    }

    file.close();
    if (count > 0) {
        cout << count << " lagu berhasil dimuat dari file." << endl;
    } else {
        cout << "Tidak ada lagu baru yang ditambahkan dari file." << endl;
    }
}

// simpan playlist ke txt
void savePlaylistToFile(Song* head, const string& fileName) {
    ofstream file(fileName);
    if (!file.is_open()) {
        cout << "Gagal membuka file: " << fileName << endl;
        return;
    }

    Song* temp = head;
    while (temp != nullptr) {
        file << temp->name << "," << temp->artist << "," << temp->genre << endl;
        temp = temp->next;
    }

    file.close();
    cout << "Playlist berhasil disimpan ke file: " << fileName << endl;
}

// menambahkan lagu yg tersedia dari user
void addSongFromUserInput(Song*& availableSongs) {
    string name, artist, genre;

    cout << "Masukkan nama lagu: ";
    getline(cin, name);
    
    cout << "Masukkan nama artis: ";
    getline(cin, artist);
    
    cout << "Masukkan genre lagu: ";
    getline(cin, genre);

    if (addSongToPlaylist(availableSongs, name, artist, genre)) {
        cout << "Lagu berhasil ditambahkan ke daftar lagu yang tersedia!" << endl;
    } else {
        cout << "Lagu tersebut sudah ada dalam daftar lagu yang tersedia." << endl;
    }
}

// menambahkan lagu ke Favorit
void addSongToFavorite(Song* availableSongs, Song*& favoriteSongs, Playlist* playlists) {
    int songChoice;
    do {
        cout << "\nDaftar Lagu Tersedia:" << endl;
        displaySongs(availableSongs);
        cout << "Pilih nomor lagu untuk ditambahkan ke playlist Favorite (0 untuk selesai): ";
        cin >> songChoice;
        if (songChoice > 0) {
            Song* temp = availableSongs;
            int index = 1;
            while (temp != nullptr && index < songChoice) {
                temp = temp->next;
                index++;
            }
            if (temp != nullptr) {
                if (addSongToPlaylist(favoriteSongs, temp->name, temp->artist, temp->genre)) {
                    Playlist* favoritePlaylist = playlists;
                    while (favoritePlaylist != nullptr) {
                        if (favoritePlaylist->name == "Favorite") {
                            favoritePlaylist->songs = favoriteSongs;
                            break;
                        }
                        favoritePlaylist = favoritePlaylist->next;
                    }
                }
            } else {
                cout << "Pilihan tidak valid." << endl;
            }
        }
    } while (songChoice != 0);
}


int main() {
    Song* availableSongs = nullptr; // lagu yang tersedia
    Playlist* playlists = nullptr; // playlist
    Song* favoriteSongs = nullptr; // playlist Favorite
    initializeAvailableSongs(availableSongs); // lagu awal yg tersedia

    // Tambah playlist "Favorite" ke daftar playlist
    Playlist* favoritePlaylist = createPlaylist("Favorite");
    favoritePlaylist->next = playlists;
    playlists = favoritePlaylist;

    int choice;
    do {
        cout << "\n=======================================" << endl;
        cout << "           🎵 Mini Spotify 🎵          " << endl;
        cout << "=======================================" << endl;
        cout << "1. Tampilkan Daftar Lagu Tersedia 🎶" << endl;
        cout << "2. Buat Playlist Baru ➕" << endl;
        cout << "3. Tampilkan Semua Playlist 📋" << endl;
        cout << "4. Muat Lagu dari File 📂" << endl;
        cout << "5. Simpan Playlist ke File 💾" << endl;
        cout << "6. Masukkan Lagu ➕" << endl;
        cout << "7. Tambahkan Lagu ke Playlist Favorite ⭐" << endl;
        cout << "0. Keluar 🚪" << endl;
        cout << "=======================================" << endl;
        cout << "Pilih opsi: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            cout << "\n=== Daftar Lagu Tersedia ===" << endl;
            displaySongs(availableSongs);
            break;
        }

        case 2: {
            string playlistName;
            cout << "Masukkan nama playlist baru: ";
            getline(cin, playlistName);

            Playlist* newPlaylist = createPlaylist(playlistName);
            newPlaylist->next = playlists;
            playlists = newPlaylist;

            int songChoice;
            do {
                cout << "\nDaftar Lagu Tersedia:" << endl;
                displaySongs(availableSongs);
                cout << "Pilih nomor lagu untuk ditambahkan ke playlist (0 untuk selesai): ";
                cin >> songChoice;
                if (songChoice > 0) {
                    Song* temp = availableSongs;
                    int index = 1;
                    while (temp != nullptr && index < songChoice) {
                        temp = temp->next;
                        index++;
                    }
                    if (temp != nullptr) {
                        addSongToPlaylist(newPlaylist->songs, temp->name, temp->artist, temp->genre);
                    } else {
                        cout << "Pilihan tidak valid." << endl;
                    }
                }
            } while (songChoice != 0);
            break;
        }

        case 3: {
    if (playlists == nullptr) {
        cout << "Tidak ada playlist yang tersedia." << endl;
        continue;
    }

    cout << "\nDaftar Playlist:" << endl;
    displayPlaylists(playlists);

    int playlistChoice;
    cout << "Pilih nomor playlist untuk melihat lagu-lagunya (0 untuk kembali ke menu utama): ";
    cin >> playlistChoice;
    cin.ignore();

    if (playlistChoice == 0) {
        break;
    }

    Playlist* selectedPlaylist = playlists;
    int index = 1;
    while (selectedPlaylist != nullptr && index < playlistChoice) {
        selectedPlaylist = selectedPlaylist->next;
        index++;
    }

    if (selectedPlaylist != nullptr) {
        int subChoice;
        do {
            cout << "\nLagu-lagu dalam Playlist: " << selectedPlaylist->name << endl;
            displaySongs(selectedPlaylist->songs);

                    cout << "\n=== Opsi Playlist ===" << endl;
                    cout << "1. Sort lagu berdasarkan nama lagu" << endl;
                    cout << "2. Sort lagu berdasarkan nama artis" << endl;
                    cout << "3. Hapus lagu dari playlist" << endl;
                    cout << "4. Tambahkan lagu ke playlist" << endl;
                    cout << "0. Kembali ke menu utama" << endl;
                    cout << "Pilih opsi: ";
                    cin >> subChoice;
                    cin.ignore();

                    switch (subChoice) {
                        case 1:
                            sortSongsByName(selectedPlaylist->songs);
                            cout << "Lagu-lagu dalam playlist telah diurutkan berdasarkan nama." << endl;
                            break;
                        case 2:
                            sortSongsByArtist(selectedPlaylist->songs);
                            cout << "Lagu-lagu dalam playlist telah diurutkan berdasarkan nama artis." << endl;
                            break;
                        case 3: {
                            string songToDelete;
                            cout << "Masukkan nama lagu yang ingin dihapus: ";
                            getline(cin, songToDelete);
                            deleteSongFromPlaylist(selectedPlaylist->songs, songToDelete);
                            break;
                        }
                        case 4: {
                            cout << "\n=== Daftar Lagu Tersedia ===" << endl;
                            displaySongs(availableSongs);

                            int songChoice;
                            cout << "Pilih nomor lagu untuk ditambahkan ke playlist (0 untuk batal): ";
                            cin >> songChoice;
                            cin.ignore();

                            if (songChoice == 0) {
                                cout << "Batal menambahkan lagu ke playlist." << endl;
                                break;
                            }

                            Song* temp = availableSongs;
                            int index = 1;
                            while (temp != nullptr && index < songChoice) {
                                temp = temp->next;
                                index++;
                            }

                            if (temp != nullptr) {
                                if (addSongToPlaylist(selectedPlaylist->songs, temp->name, temp->artist, temp->genre)) {
                                    cout << "Lagu '" << temp->name << "' berhasil ditambahkan ke playlist '" << selectedPlaylist->name << "'." << endl;
                                } else {
                                    cout << "Lagu tersebut sudah ada dalam playlist." << endl;
                                }
                            } else {
                                cout << "Pilihan tidak valid." << endl;
                            }
                            break;
                        }
                        case 0:
                            cout << "Kembali ke menu utama..." << endl;
                            break;
                        default:
                            cout << "Pilihan tidak valid. Coba lagi." << endl;
                    }
                } while (subChoice != 0);
            } else {
                cout << "Pilihan tidak valid!" << endl;
            }
            break;
        }

        case 4: {
            string fileName;
            cout << "Masukkan nama file untuk membaca lagu (dengan .txt): ";
            cin >> fileName;

            if (fileName.find(".txt") == string::npos) {
                cout << "Harap masukkan file dengan format .txt" << endl;
            } else {
                readFromFile(availableSongs, fileName);
            }
            break;
        }

        case 5: {
            if (playlists == nullptr) {
                cout << "Tidak ada playlist yang tersedia untuk disimpan." << endl;
            } else {
                int playlistChoice;
                cout << "\nDaftar Playlist:" << endl;
                displayPlaylists(playlists);
                cout << "Pilih nomor playlist untuk disimpan: ";
                cin >> playlistChoice;

                Playlist* temp = playlists;
                int index = 1;
                while (temp != nullptr && index < playlistChoice) {
                    temp = temp->next;
                    index++;
                }

                if (temp != nullptr) {
                    string fileName;
                    cout << "Masukkan nama file untuk menyimpan playlist (dengan .txt): ";
                    cin >> fileName;
                    savePlaylistToFile(temp->songs, fileName);
                } else {
                    cout << "Pilihan tidak valid." << endl;
                }
            }
            break;
        }

        case 6: {
            addSongFromUserInput(availableSongs);
            break;
        }

        case 7: {
            addSongToFavorite(availableSongs, favoriteSongs, playlists);
            break;
        }

        case 0:{
            cout << "\nTerima kasih telah menggunakan Mini Spotify!" << endl;
            break;
        }

        default:
            cout << "Pilihan tidak valid! Coba lagi." << endl;
        }
    } while (choice != 0);

    return 0;
}
