#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
// yaskur
// Struktur untuk menyimpan data lagu
struct Song {
    string name;
    string artist;
    string genre;
    Song* next;
};

// Struktur untuk playlist
struct Playlist {
    string name;
    Song* songs;
    Playlist* next;
};

// Fungsi untuk membuat node lagu baru
Song* createSong(string name, string artist, string genre) {
    Song* newSong = new Song();
    newSong->name = name;
    newSong->artist = artist;
    newSong->genre = genre;
    newSong->next = nullptr;
    return newSong;
}

// Fungsi untuk membuat node playlist baru
Playlist* createPlaylist(string name) {
    Playlist* newPlaylist = new Playlist();
    newPlaylist->name = name;
    newPlaylist->songs = nullptr;
    newPlaylist->next = nullptr;
    return newPlaylist;
}

//untuk menambahkan lagu
void addSongToPlaylist(Song*& playlistSongs, string name, string artist, string genre) {
    // Mengecek apakah lagu sudah ada dalam playlist
    Song* temp = playlistSongs;
    while (temp != nullptr) {
        if (temp->name == name && temp->artist == artist) {
            cout << "Lagu '" << name << "' sudah ada di dalam playlist ini." << endl;
            return; // Jangan lanjutkan menambahkan lagu jika sudah ada
        }
        temp = temp->next;
    }

    // Jika lagu belum ada, tambahkan ke playlist
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
    cout << "Lagu '" << name << "' berhasil ditambahkan ke playlist." << endl; // Hanya ditampilkan jika lagu berhasil ditambahkan
}


// Fungsi untuk menampilkan semua lagu dalam playlist
void displaySongs(Song* head) {
    if (head == nullptr) {
        cout << "Playlist kosong!" << endl;
        return;
    }
    Song* temp = head;
    int index = 1;
    while (temp != nullptr) {
        cout << index << ". " << temp->name << " - " << temp->artist << " (" << temp->genre << ")" << endl;
        temp = temp->next;
        index++;
    }
}

// Fungsi untuk menampilkan semua playlist yang ada
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

// Fungsi untuk menginisialisasi daftar lagu
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

// Fungsi untuk mengonversi linked list lagu menjadi array untuk sorting
int songListToArray(Song* head, Song* arr[]) {
    int count = 0;
    while (head != nullptr) {
        arr[count++] = head;
        head = head->next;
    }
    return count;
}

// Fungsi untuk mengurutkan playlist berdasarkan nama lagu
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

// Fungsi untuk mengurutkan playlist berdasarkan nama artis
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

// Fungsi untuk menghapus lagu dari playlist
void deleteSongFromPlaylist(Song*& head, string songName) {
    if (head == nullptr) {
        cout << "Playlist kosong! Tidak ada lagu yang dapat dihapus." << endl;
        return;
    }

    // Jika lagu yang ingin dihapus adalah head
    if (head->name == songName) {
        Song* temp = head;
        head = head->next;
        delete temp;
        cout << "Lagu '" << songName << "' berhasil dihapus dari playlist." << endl;
        return;
    }

    // Mencari lagu dalam playlist
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

    // Hapus node yang ditemukan
    previous->next = current->next;
    delete current;
    cout << "Lagu '" << songName << "' berhasil dihapus dari playlist." << endl;
}


int main() {
    Song* availableSongs = nullptr; // Daftar lagu yang tersedia
    Playlist* playlists = nullptr; // Daftar playlist
    initializeAvailableSongs(availableSongs); // Inisialisasi daftar lagu

    int choice;
    do {
        cout << "\n=== Menu Playlist ===" << endl;
        cout << "1. Tampilkan Daftar Lagu Tersedia" << endl;
        cout << "2. Buat Playlist Baru" << endl;
        cout << "3. Tampilkan Semua Playlist" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilih opsi: ";
        cin >> choice;

        cin.ignore(); // Membersihkan buffer input
        if (choice == 1) {
            cout << "\nDaftar Lagu Tersedia:" << endl;
            displaySongs(availableSongs);
        } else if (choice == 2) {
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
        } else if (choice == 3) {
            if (playlists == nullptr) {
                cout << "Tidak ada playlist yang tersedia." << endl;
                continue;
            }

            cout << "\nDaftar Playlist:" << endl;
            displayPlaylists(playlists);

            int playlistChoice;
            cout << "Pilih nomor playlist untuk ditampilkan: ";
            cin >> playlistChoice;

            Playlist* temp = playlists;
            int index = 1;
            while (temp != nullptr && index < playlistChoice) {
                temp = temp->next;
                index++;
            }

            if (temp != nullptr) {
                int subChoice;
                do {
                    cout << "\nPlaylist: " << temp->name << endl;
                    cout << "Lagu-lagu dalam playlist:" << endl;
                    displaySongs(temp->songs);

                    cout << "\n1. Urutkan berdasarkan Nama Lagu" << endl;
                    cout << "2. Urutkan berdasarkan Nama Artis" << endl;
                    cout << "3. Tambahkan Lagu ke Playlist" << endl;
                    cout << "4. Hapus Lagu dari Playlist" << endl;
                    cout << "0. Kembali ke menu utama" << endl;
                    cout << "Pilih opsi: ";
                    cin >> subChoice;

                    if (subChoice == 1) { // mengurutkan lagu berdasarkan nama
                        sortSongsByName(temp->songs);
                        cout << "Playlist diurutkan berdasarkan Nama Lagu." << endl;
                    } else if (subChoice == 2) { // mengurutkan lagu berdasarkan nama artis
                        sortSongsByArtist(temp->songs);
                        cout << "Playlist diurutkan berdasarkan Nama Artis." << endl;
                    } else if (subChoice == 3) { // menambahkan lagu ke playlist yang sudah dibuat
                        int songChoice;
                        do {
                            cout << "\nDaftar Lagu Tersedia:" << endl;
                            displaySongs(availableSongs);
                            cout << "Pilih nomor lagu untuk ditambahkan ke playlist (0 untuk selesai): ";
                            cin >> songChoice;

                            if (songChoice > 0) {
                                Song* songTemp = availableSongs;
                                int index = 1;
                                while (songTemp != nullptr && index < songChoice) {
                                    songTemp = songTemp->next;
                                    index++;
                                }
                                if (songTemp != nullptr) {
                                    addSongToPlaylist(temp->songs, songTemp->name, songTemp->artist, songTemp->genre);
                                }
                            }
                        } while (songChoice != 0);
                    } 
                    
                    else if (subChoice == 4) { // Hapus lagu dari playlist yang sudah dibuat
                        cin.ignore(); // Membersihkan buffer
                        string songName;
                        cout << "Masukkan nama lagu yang ingin dihapus: ";
                        getline(cin, songName);
                        deleteSongFromPlaylist(temp->songs, songName);
                    } 
                    
                    else if (subChoice != 0) {
                        cout << "Pilihan tidak valid!" << endl;
                    }
                } while (subChoice != 0);
            } else {
                cout << "Pilihan tidak valid." << endl;
            }
        } else if (choice == 0) {
            cout << "Keluar dari program." << endl;
        } else {
            cout << "Pilihan tidak valid!" << endl;
        } 
    } while (choice != 0);

    return 0;
}
