# Rancang-Bangun-Sistem-Alat-Kendali-Kualitas-Udara-Menggunakan-Android-Studio
memantau dan mengatur kualitas udara dalam ruangan melalui aplikasi Android

Latar Belakang
Kualitas udara dalam ruangan memiliki dampak besar pada kesehatan dan kenyamanan manusia sehari-hari. Udara yang tercemar dapat menyebabkan gangguan pernapasan, alergi, dan dampak negatif lainnya. Karena banyaknya waktu yang dihabiskan di dalam ruangan, baik di tempat tinggal maupun tempat kerja, pengendalian kualitas udara menjadi penting. Diperlukan alat Sistem Kendali Kualitas Udara dengan sensor MQ 135 untuk mendeteksi udara tidak sehat, sensor GP2Y1010AU0F untuk sensitivitas debu, dan penambahan filter H13 atau HEPA untuk menyaring udara dari kotoran

Menghubungkan alat melalui socket AC ke sumber listrik.
1. Menghubungkan ESP 32 dengan jaringan Wi-Fi yang memiliki akses internet
2. Buka aplikasi "Freshism" yang sudah diinstal pada smartphone
3. Sensor akan membaca nilai udara dan debu dan hasilnya akan tampil di aplikasi
4. 4. Tekan Tombol ON untuk mengaktifkan Kipas DC
5. Pilih Mode "Auto" pada tombol aplikasi "Freshism"
6. jika indikator kualitas udara tidak sehat, maka indikator led akan berwarna merah, tampilan aplikasi tidak sehat, dan kipas dc menyala
