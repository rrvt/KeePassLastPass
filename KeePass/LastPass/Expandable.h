// Expandable Array
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

// Data with destructors must have a "clear()" method (function) that zeros all data components.

#pragma once


template <class Data, const int n>

class Expandable {
int   endN;
int   tblN;
Data* tbl;

public:

  Expandable() {tbl = 0; endN = 0; tblN = n; tbl = new Data[n+2];}

 ~Expandable() {if (tbl) delete [] tbl; tbl = 0; endN = tblN = 0;}

  Data& operator[] (int i) {
    if (i >= tblN) expand(i);
    if (i >= endN) endN = i+1;
    return tbl[i];
    }

  void clr() {endN = 0;}            // Clears the number of items in array (without deleting data)
  int  get() {return endN++;}       // Returns next item and increments
  int  end() {return endN;}         // Returns number of items in array if inserted sequentially


  // Insert Data d into array sorted (being sure to expand it if necessary.  Note, if one use [] to insert
  // data into array, sorting is up to the user...!

  Expandable& operator= (Data& d) {
  Data xNode;
  Data nextNode;
  int  i;

    for (i = 0; i < endN; i++) if (tbl[i] >= d) {if (tbl[i] == d) return *this; break;}

    for (xNode = d, endN++; i < endN; i++) {

      if (i >= tblN) expand(i);   nextNode = tbl[i]; tbl[i] = xNode; xNode = nextNode;
      }
    return *this;
    }

  // Insert data at index, moving other entries out of the way

  void operator() (int x, Data& d) {
  int i;

    if (++endN >= tblN) expand(endN);

    for (i = endN-2; i >= x; i--) tbl[i+1] = tbl[i];

    tbl[x] = d;
    }

  void del(int x) {
  int i;

    if (x < 0 || endN <= x) return;

    for (i = x, --endN; i < endN; i++) tbl[i] = tbl[i+1];
    }

private:

  void expand(int i) {
  Data* p = tbl;
  Data* q = tbl;
  int   n = tblN;
  int   j;

    for (j = 0; tblN <= i && tblN < INT_MAX/2; j++) tblN = tblN ? tblN * 2 : 1;

    tbl = new Data[tblN+2];

    for (j = 0; j < n; j++, p++) {tbl[j] = *p; ZeroMemory(p, sizeof(Data));}

    delete [] q;
    }
  };

