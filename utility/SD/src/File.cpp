/*

  SD - a slightly more friendly wrapper for sdfatlib

  This library aims to expose a subset of SD card functionality
  in the form of a higher level "wrapper" object.

  License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

  (C) Copyright 2010 SparkFun Electronics

*/

#include <SD.h>

/* for debugging file open/close leaks
   uint8_t nfilecount=0;
*/

File2::File2(SdFile2 f, const char *n) {
  // oh man you are kidding me, new() doesn't exist? Ok we do it by hand!
  _file = (SdFile2 *)malloc(sizeof(SdFile2));
  if (_file) {
    memcpy(_file, &f, sizeof(SdFile2));

    strncpy(_name, n, 12);
    _name[12] = 0;

    /* for debugging file open/close leaks
       nfilecount++;
       Serial.print("Created \"");
       Serial.print(n);
       Serial.print("\": ");
       Serial.println(nfilecount, DEC);
    */
  }
}

File2::File2(void) {
  _file = 0;
  _name[0] = 0;
  //Serial.print("Created empty file object");
}

// returns a pointer to the file name
char *File2::name(void) {
  return _name;
}

SdFile2 *File2::getFile() {
  return _file;
}

// a directory is a special type of file
bool File2::isDirectory(void) {
  return (_file && _file->isDir());
}


size_t File2::write(uint8_t val) {
  return write(&val, 1);
}

size_t File2::write(const uint8_t *buf, size_t size) {
  size_t t;
  if (!_file) {
    setWriteError();
    return 0;
  }
  _file->clearWriteError();
  t = _file->write(buf, size);
  if (_file->getWriteError()) {
    setWriteError();
    return 0;
  }
  return t;
}

int File2::availableForWrite() {
  if (_file) {
    return _file->availableForWrite();
  }
  return 0;
}

int File2::peek() {
  if (! _file) {
    return 0;
  }

  int c = _file->read();
  if (c != -1) {
    _file->seekCur(-1);
  }
  return c;
}

int File2::read() {
  if (_file) {
    return _file->read();
  }
  return -1;
}

// buffered read for more efficient, high speed reading
int File2::read(void *buf, uint16_t nbyte) {
  if (_file) {
    return _file->read(buf, nbyte);
  }
  return 0;
}

int File2::available() {
  if (! _file) {
    return 0;
  }

  uint32_t n = size() - position();

  return n > 0X7FFF ? 0X7FFF : n;
}

void File2::flush() {
  if (_file) {
    _file->sync();
  }
}

bool File2::seek(uint32_t pos) {
  if (! _file) {
    return false;
  }

  return _file->seekSet(pos);
}

uint32_t File2::position() {
  if (! _file) {
    return -1;
  }
  return _file->curPosition();
}

uint32_t File2::size() {
  if (! _file) {
    return 0;
  }
  return _file->fileSize();
}

void File2::close() {
  if (_file) {
    _file->close();
    free(_file);
    _file = 0;

    /* for debugging file open/close leaks
      nfilecount--;
      Serial.print("Deleted ");
      Serial.println(nfilecount, DEC);
    */
  }
}

File2::operator bool() {
  if (_file) {
    return  _file->isOpen();
  }
  return false;
}

