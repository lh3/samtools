#ifndef KNETFILE_H
#define KNETFILE_H

typedef struct knetFile_s knetFile;

#ifdef KNETFILE_HOOKS
// the following allow knetfile to wrap an alternate I/O library
typedef knetFile *(*knet_alt_open_f)(const char *fn, const char *mode);
typedef knetFile *(*knet_alt_dopen_f)(int fd, const char *mode);
typedef off_t (*knet_alt_read_f)(knetFile *fp, void *buf, off_t len);
typedef off_t (*knet_alt_seek_f)(knetFile *fp, int64_t off, int whence);
typedef off_t (*knet_alt_tell_f)(knetFile *fp);
typedef int (*knet_alt_close_f)(knetFile *fp);
#else 
// As of 2/18/2010 this is not used anywhere in samtools, and would not play well with abstraction:
#define knet_fileno(fp) ((fp)->fd)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	int knet_win32_init();
	void knet_win32_destroy();
#endif

	knetFile *knet_open(const char *fn, const char *mode);

#ifdef KNETFILE_HOOKS
	void knet_init_alt(knet_alt_open_f open, knet_alt_dopen_f dopen, knet_alt_read_f read,
					   knet_alt_seek_f seek, knet_alt_tell_f tell, knet_alt_close_f close);
#endif

	/* 
	   This only works with local files.
	 */
	knetFile *knet_dopen(int fd, const char *mode);

	/*
	  If ->is_ready==0, this routine updates ->fd; otherwise, it simply
	  reads from ->fd.
	 */
	off_t knet_read(knetFile *fp, void *buf, off_t len);

	/*
	  This routine only sets ->offset and ->is_ready=0. It does not
	  communicate with the FTP server.
	 */
	off_t knet_seek(knetFile *fp, int64_t off, int whence);
	off_t knet_tell(knetFile *fp);
	int knet_close(knetFile *fp);

#ifdef __cplusplus
}
#endif

#endif
