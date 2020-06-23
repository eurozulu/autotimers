/**
 * Open source library
 * Copyright Rob Gilham 2020
 */
 // Reads the given stream as Hex char encoded bytes.
// so a stream of 010CFF would yeild bytes 01, 12, 255
// Chars are always read in Pairs, with the exception of the last one, if an odd number of chars is found, then a '0' is inserted before the last char.
// Carrage returnas and linefeeds are ignored
byte readHexEncodedSerial(const Stream& stream, byte buf[], byte bSize) {
  byte cBuf[255];
  byte len = readSerial(stream, cBuf, 255);
  if (len < 1) {
    return 0;
  }

  int index = 0;
  byte offset = 0;
  while (index < len) {
    if (cBuf[index] == '\n' || cBuf[index] == '\r' || cBuf[index] == ' ') {
      index ++;
      continue;
    }
    
    buf[offset] = char2byte(cBuf[index]);
    index++;
    
    if (index < len) { // Add following char to the current byte, if it exists.
      buf[offset] = (buf[offset] << 4) + char2byte(cBuf[index]);
      index++;
    }
    offset++;
  }
  return offset;
}

// Reads the given Stream as long as it has #available() bytes and given buf is not full.
byte readSerial(const Stream& stream, byte buf[], byte bSize) {
  byte len = 0;
  
  while (stream.available() > 0 && len < bSize) {
    buf[0 + len] = stream.read();
    len++;
    
    if (!stream.available()) // pause to wait for next char arriving.
      delay(100);
  }
  return len;
}


// convert the given char of a hex number, into its byte value.
byte char2byte(char input) {
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;
}
