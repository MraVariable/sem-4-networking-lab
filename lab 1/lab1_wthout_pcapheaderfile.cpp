// HexDump.cpp
// This code has been tested with Visual C++ 6.0 SP5 and with g++ 3.0.4.
//
// Invocation:  HexDump <file to display>
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

int main(int argc, char** argv) {

   char* Byte = new char;
   int   byteCount = 0;

   ifstream binFile("lab1packet.pcap", ios::in | ios::binary); // open input file in binary mode
   if ( binFile.fail() ) {
      cout << "Unable to find file: " << argv[1] << endl;
      return 1;
   }

   cout << "Hex dump of file " << "lab1packet.pcap" << ":" << endl << endl;
   const string colHeader = "  00  01  02  03  04  05  06  07      08  09  0A  0B  0C  0D  0E  0F";
   const string Delimiter = "  ------------------------------------------------------------------";
   cout << colHeader << endl;
   cout << Delimiter << endl;

   binFile.read(Byte, 1);                   // grab first byte of file
   cout << setfill('0') << uppercase;       // set fill character to handle single-digit hex values
                                            //     and display upper hex digits in upper-case
   while ( binFile ) {                      // continue until input stream fails

      cout << "  " << hex << setw(2)        // write in hex mode, with separation
           << (short(*Byte) & 0x00FF);      // cast char* to short (for two-byte value)
                                            // mask upper byte of short with zeros
      byteCount++;
      if (byteCount % 8 == 0) {
         if (byteCount % 16 == 0)
            cout << endl;
         else
            cout << "    ";
      }
      binFile.read(Byte, 1);                 // grab next byte of file
   }
   cout << endl;
   cout << Delimiter << endl;
   cout << colHeader << endl;

   binFile.close();
   return 0;
}
