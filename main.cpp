#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define MAX_CHAR 256

using namespace std;


class KodeSimbol {
private:
   char simbol;
   int frekuensi;
   string kode;
public:
   KodeSimbol( char in_symbol, int in_frequency );
   char getSymbol( void );
   int getFrequency( void );
   string getCode( void );
   void addCode( string in_code );
};

KodeSimbol::KodeSimbol( char in_simbol, int in_frekuensi ) {
   simbol = in_simbol;
   frekuensi = in_frekuensi;
   kode = "";
}

char KodeSimbol::getSymbol( void ) {
   return simbol;
}

int KodeSimbol::getFrequency( void ) {
   return frekuensi;
}

string KodeSimbol::getCode( void ) {
   return kode;
}

void KodeSimbol::addCode( string in_kode ) {
   kode += in_kode;
}


// proses penyimpanan simbol terurut berdasarkan frekuensi yang tidak bernilai nol
vector<KodeSimbol> symbol;

// Frekuensi
char frequency[ MAX_CHAR ];

void printKodeSimbol( KodeSimbol in_symbol ) {
   cout << "Symbol: " << in_symbol.getSymbol();
   cout << " Freq: " << in_symbol.getFrequency();
   cout << " Code: " << in_symbol.getCode() << endl;
}

void shannonFano( int in_begin, int in_end ) {
   // kondisi parameter tidak valid
   if ( in_begin >= in_end ) {
      return;
   }

   // tersisa 2 simbol 
   if ( in_begin == in_end - 1 ) {
      symbol.at( in_begin ).addCode( "0" );
      symbol.at( in_end ).addCode( "1" );
      return;
   }

   int highPtr = in_begin; // pointertinggi menuju kebawah
   int lowPtr = in_end;
   int highSum = symbol.at( highPtr ).getFrequency();
   int lowSum = symbol.at( lowPtr ).getFrequency();

   // memindahkan pointertinggi menuju bawah dan pointerrendah menuju atas
   while ( highPtr != lowPtr - 1 ) {
      if ( highSum > lowSum ) {
         lowPtr --;
         lowSum += symbol.at( lowPtr ).getFrequency();
      } else {
         highPtr ++;
         highSum += symbol.at( highPtr ).getFrequency();
      }
   }

   int i;
   for ( i=in_begin; i<=highPtr; i++ ) {
      symbol.at( i ).addCode( "0" );
   }
   for ( i=lowPtr; i<=in_end; i++ ) {
      symbol.at( i ).addCode( "1" );
   }

   shannonFano( in_begin, highPtr );
   shannonFano( lowPtr, in_end );
}

int main () {
   FILE * pFile;
   int c;
   int n = 0;
   pFile = fopen("text1.txt","r");
   if (pFile==NULL) {
      perror ("Error opening file");
   } else {
      int i;
      for ( i=0; i<MAX_CHAR; i++ ) {
         frequency[ i ] = 0;
      }
      do {
         c = fgetc (pFile);
         if ( c != EOF ) {
            frequency[ c ] ++;
            printf( "%c", c );
         }
      } while (c != EOF);
      fclose (pFile);
   }

   // menampilkan informasi frekuensi
   cout << endl << endl;
   int i;
   for ( i=0; i<MAX_CHAR; i++ ) {
      if ( frequency[ i ] != 0 ) {
         printf( "%d, %c: %d\n", i, i, frequency[ i ] );
      }
   }

   // memasukan dan mengurutkan simbol
   printf( "\n\n" );
   for ( i=0; i<MAX_CHAR; i++ ) {
      if ( frequency[ i ] != 0 ) {
         if ( symbol.empty() ) {
            KodeSimbol newSymbol( i, frequency[ i ] );
            symbol.push_back( newSymbol );
         } else {
            int j;
            vector<KodeSimbol>::iterator it;
            it = symbol.begin();
            for ( j=0; j<symbol.size(); j++ ) {
               if ( frequency[ i ] >= symbol[ j ].getFrequency() ) {
                  KodeSimbol newSymbol( i, frequency[ i ] );
                  symbol.insert( it, newSymbol );
                  break;
               } else if ( j == symbol.size() - 1 ) {
                  KodeSimbol newSymbol( i, frequency[ i ] );
                  symbol.push_back( newSymbol );
                  break;
               }
               it ++;
            }
         }
      }
   }

   shannonFano( 0, symbol.size() - 1 );
  
   printf( "\n\n \n" );
   for_each( symbol.begin(), symbol.end(), printKodeSimbol );
   printf( "\n\n " );

   return 0;
}
