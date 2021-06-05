# soal-shift-sisop-modul-4-E01-2021

## Soal No.1

Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :

```
NOTE : 
Semua file yang berada pada direktori harus ter-encode menggunakan Atbash cipher(mirror).
Misalkan terdapat file bernama kucinglucu123.jpg pada direktori DATA_PENTING
“AtoZ_folder/DATA_PENTING/kucinglucu123.jpg” → “AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg”
Note : filesystem berfungsi normal layaknya linux pada umumnya, Mount source (root) filesystem adalah directory /home/[USER]/Downloads, dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di-encode.
Referensi : https://www.dcode.fr/atbash-cipher
```

### (a)
Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

### (b)
Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

### (c)
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

### (d)
Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]

### (e)
Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

## Penyelesaian No. 1

### 1.a

## Soal No.2
Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang dirancang oleh Sei:

### 2.a 
Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).

### 2.b 
Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).

### 2.c
Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.

### 2.d
Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).

### 2.e
Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:

Suatu_File.txt.0000
Suatu_File.txt.0001
Suatu_File.txt.0002

Ketika diakses melalui filesystem hanya akan muncul Suatu_File.txt

## Penyelesaian No.2 :

Tidak Terjawab

## Soal No.3
Karena Sin masih super duper gabut akhirnya dia menambahkan sebuah fitur lagi pada filesystem mereka. 

### 3.a
Jika sebuah direktori dibuat dengan awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

### 3.b
Jika sebuah direktori di-rename dengan memberi awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

### 3.c
Apabila direktori yang terenkripsi di-rename dengan menghapus “A_is_a_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.

### 3.d
Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ_” maupun “RX_” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive  “AtoZ_” dan “RX_” tetap berjalan pada subdirektori).

### 3.e
Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase insensitive dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.


Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.


## Penyelesaian No.3 :

Tidak Terjawab

## Soal No.4
Untuk memudahkan dalam memonitor kegiatan pada filesystem mereka Sin dan Sei membuat sebuah log system dengan spesifikasi sebagai berikut.

### 4.a
Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.

### 4.b
Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.

### 4.c
Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.

### 4.d
Sisanya, akan dicatat pada level INFO.

### 4.e
Format untuk logging yaitu:
```
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]
```

Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan
```
INFO::28052021-10:00:00:CREATE::/test.txt
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt
```

Catatan: 
- Tidak diperkenankan menggunakan system() dan exec*(), kecuali ada pengecualian di butir soal.
- Pengerjaan hanya dilakukan dalam 1 file program C dengan format nama SinSeiFS_[Kelompok].c . 


## Penyelesaian No.4 :
Sebagian besar kodingan program ini terdapat pada fungsi `log_v2` yang mengambil inp .

### 4.a
Untuk soal ini, kita menentukan nama directory.
```
static char *logpath = "/home/wisnupramoedya/SinSeiFS.log";
```

Kita membuat log_file berikut.
```
FILE *log_file = fopen(logpath, "a");
```

### 4.b
Kita membuat dua jenis kode log.
```
const int INFO = 1;
const int WARNING = 2;
```

### 4.c
Pada rmdir, kode yang ditulis.
```
char str[100];
sprintf(str, "RMDIR::%s", path);
log_v2(str, WARNING);
```

Sedangkan, unlink sebagai berikut.
```
char str[100];
sprintf(str, "REMOVE::%s", path);
log_v2(str, WARNING);
```

### 4.d
Sedangkan, pada yang lain, akan seragam sebagai berikut.
```
char str[100];
sprintf(str, "MKDIR::%s", path);
log_v2(str, INFO);
```

### 4.e
Untuk menulis logging, dilakukan cara berikut pada log_v2.
```
    time_t current_time;
    time(&current_time);
    struct tm *time_info;
    time_info = localtime(&current_time);

    if (type == INFO) {
        fprintf(log_file, "INFO::%d%d%d-%d:%d:%d:%s\n", time_info->tm_mday,
                time_info->tm_mon, time_info->tm_year, time_info->tm_hour,
                time_info->tm_min, time_info->tm_sec, str);
    } else if (type == WARNING) {
        fprintf(log_file, "WARNING::%d%d%d-%d:%d:%d:%s\n", time_info->tm_mday,
                time_info->tm_mon, time_info->tm_year, time_info->tm_hour,
                time_info->tm_min, time_info->tm_sec, str);
    }
```

## Kendala
- FUSE sangat sedikit dokumentasi
- Masih bingung konsep FUSE dan paling susah diurusan debugging
