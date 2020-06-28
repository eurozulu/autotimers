/**
   Open source library
   Copyright Rob Gilham 2020
*/
uint32_t readNumber(const Stream& stream) {
  const char* input = readString(stream);
  if (input == 0)
    return 0;  
  long l = atol(input);
  delete input;
  return l;
}


const char* readString(const Stream& stream) {
  int bSize = 128;
  int len = 0;
  char *cBuf = new char[bSize];
  while (stream.available() > 0 && len < bSize) {
    cBuf[len] = (char)stream.read();
    len++;
    if (!stream.available()) // pause to wait for next char arriving.
      delay(100);
  }
  if (len < 1) {
    delete cBuf;
    return 0;
  }

  // Ensure trailing zero present
  if (len < bSize && cBuf[len - 1] != 0) {
    cBuf[len] = 0;
    len++;
  }

  return cBuf;
}
