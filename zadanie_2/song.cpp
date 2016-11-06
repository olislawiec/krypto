#include <mpg123.h>
#include <ao/ao.h>
#include "song.hpp"

#define BITS 8

//Travis/CircleCI hax
#if MPG123_API_VERSION < 40
int mpg123_encsize(int encoding) {
    return 0;
}
#endif


void song::play() {

    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char *) malloc(buffer_size * sizeof(unsigned char));

    mpg123_open_feed(mh);

    mpg123_feed(mh, (const unsigned char *) songData->data(), songData->size());

    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK && !_stop) {
        ao_play(dev, (char *) buffer, (uint_32) done);

        lock.lock();
        if (sec != 0) {
            off_t off = mpg123_timeframe(mh, sec);
            mpg123_seek_frame(mh, off, SEEK_CUR);
            sec = 0;
        }
        lock.unlock();

    }

    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

}
