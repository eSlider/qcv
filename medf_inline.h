/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */


/*
 *  This Quickselect routine is based on the algorithm described in
 *  "Numerical recipes in C", Second Edition,
 *  Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5
 *  This code by Nicolas Devillard - 1998. Public domain.
 */


#define ELEM_SWAP(a,b) { t=(a);(a)=(b);(b)=t; }
#define ELEM_SORT(a,b) { if ((a)>(b)) ELEM_SWAP((a),(b)); }

inline
int median_filter(int arr[], const int n) 
{
    register int t;
    if (n <= 9)
    {
        switch(n)
        {
            case 9:
                ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[4], arr[5]) ; ELEM_SORT(arr[7], arr[8]) ;
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[6], arr[7]) ;
                ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[4], arr[5]) ; ELEM_SORT(arr[7], arr[8]) ;
                ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[5], arr[8]) ; ELEM_SORT(arr[4], arr[7]) ;
                ELEM_SORT(arr[3], arr[6]) ; ELEM_SORT(arr[1], arr[4]) ; ELEM_SORT(arr[2], arr[5]) ;
                ELEM_SORT(arr[4], arr[7]) ; ELEM_SORT(arr[4], arr[2]) ; ELEM_SORT(arr[6], arr[4]) ;
                ELEM_SORT(arr[4], arr[2]) ; 
                return arr[4];
                break;
      
            case 8:
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[6], arr[7]) ;
                ELEM_SORT(arr[2], arr[3]) ; ELEM_SORT(arr[5], arr[6]) ; ELEM_SORT(arr[3], arr[4]) ;
                ELEM_SORT(arr[6], arr[7]) ; ELEM_SORT(arr[4], arr[7]) ; ELEM_SORT(arr[3], arr[6]) ;
                ELEM_SORT(arr[2], arr[5]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[1], arr[4]) ;
                ELEM_SORT(arr[3], arr[6]) ; ELEM_SORT(arr[3], arr[1]) ; ELEM_SORT(arr[5], arr[3]) ;
                ELEM_SORT(arr[3], arr[1]) ; 
                return arr[3];
                break;

            case 7:
                ELEM_SORT(arr[0], arr[5]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[1], arr[6]) ;
                ELEM_SORT(arr[2], arr[4]) ; ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[5]) ;
                ELEM_SORT(arr[2], arr[6]) ; ELEM_SORT(arr[2], arr[3]) ; ELEM_SORT(arr[3], arr[6]) ;
                ELEM_SORT(arr[4], arr[5]) ; ELEM_SORT(arr[1], arr[4]) ; ELEM_SORT(arr[1], arr[3]) ;
                ELEM_SORT(arr[3], arr[4]) ; 
                return arr[3];
                break;

            case 6:
                ELEM_SORT(arr[0], arr[5]) ; ELEM_SORT(arr[1], arr[3]) ; ELEM_SORT(arr[2], arr[4]) ;
                ELEM_SORT(arr[1], arr[5]) ; ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[2], arr[5]) ;
                ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[0], arr[2]) ;
                ELEM_SORT(arr[2], arr[3]) ;
                return arr[2];
                break;

            case 5:
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[0], arr[3]) ;
                ELEM_SORT(arr[1], arr[4]) ; ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[2], arr[3]) ;
                ELEM_SORT(arr[1], arr[2]) ; 
                return arr[2];
                break;
        
            case 4:
                ELEM_SORT(arr[2], arr[3]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[0], arr[1]) ;
                ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[0], arr[1]) ; 
                return arr[1];
                break;

            case 3:
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[0], arr[1]) ;
                return arr[1];
                break;

            case 2:
            case 1:
                return arr[0];
                break;

            default:
                return 0;
                break;
        }
    }
    else
    {
        int low, high ;
        int median;
        int middle, ll, hh;

        low = 0 ; high = n-1 ; median = (low + high) / 2;
        for (;;) {
            if (high <= low) /* One element only */
                return arr[median] ;

            if (high == low + 1) {  /* Two elements only */
                if (arr[low] > arr[high])
                    ELEM_SWAP(arr[low], arr[high]) ;
                return arr[median] ;
            }

            /* Find median of low, middle and high items; swap into position low */
            middle = (low + high) / 2;
            if (arr[middle] > arr[high])    ELEM_SWAP(arr[middle], arr[high]) ;
            if (arr[low] > arr[high])       ELEM_SWAP(arr[low], arr[high]) ;
            if (arr[middle] > arr[low])     ELEM_SWAP(arr[middle], arr[low]) ;

            /* Swap low item (now in position middle) into position (low+1) */
            ELEM_SWAP(arr[middle], arr[low+1]) ;

            /* Nibble from each end towards middle, swapping items when stuck */
            ll = low + 1;
            hh = high;
            for (;;) {
                do ll++; while (arr[low] > arr[ll]) ;
                do hh--; while (arr[hh]  > arr[low]) ;

                if (hh < ll)
                    break;

                ELEM_SWAP(arr[ll], arr[hh]) ;
            }

            /* Swap middle item (in position low) back into correct position */
            ELEM_SWAP(arr[low], arr[hh]) ;

            /* Re-set active partition */
            if (hh <= median)
                low = ll;
            if (hh >= median)
                high = hh - 1;
        }
    }
}

inline
float median_filter_float(float arr[], const int n) 
{
    float t;
    if (n <= 9)
    {
        switch(n)
        {
            case 9:
                ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[4], arr[5]) ; ELEM_SORT(arr[7], arr[8]) ;
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[6], arr[7]) ;
                ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[4], arr[5]) ; ELEM_SORT(arr[7], arr[8]) ;
                ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[5], arr[8]) ; ELEM_SORT(arr[4], arr[7]) ;
                ELEM_SORT(arr[3], arr[6]) ; ELEM_SORT(arr[1], arr[4]) ; ELEM_SORT(arr[2], arr[5]) ;
                ELEM_SORT(arr[4], arr[7]) ; ELEM_SORT(arr[4], arr[2]) ; ELEM_SORT(arr[6], arr[4]) ;
                ELEM_SORT(arr[4], arr[2]) ; 
                return arr[4];
                break;
      
            case 8:
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[6], arr[7]) ;
                ELEM_SORT(arr[2], arr[3]) ; ELEM_SORT(arr[5], arr[6]) ; ELEM_SORT(arr[3], arr[4]) ;
                ELEM_SORT(arr[6], arr[7]) ; ELEM_SORT(arr[4], arr[7]) ; ELEM_SORT(arr[3], arr[6]) ;
                ELEM_SORT(arr[2], arr[5]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[1], arr[4]) ;
                ELEM_SORT(arr[3], arr[6]) ; ELEM_SORT(arr[3], arr[1]) ; ELEM_SORT(arr[5], arr[3]) ;
                ELEM_SORT(arr[3], arr[1]) ; 
                return arr[3];
                break;

            case 7:
                ELEM_SORT(arr[0], arr[5]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[1], arr[6]) ;
                ELEM_SORT(arr[2], arr[4]) ; ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[5]) ;
                ELEM_SORT(arr[2], arr[6]) ; ELEM_SORT(arr[2], arr[3]) ; ELEM_SORT(arr[3], arr[6]) ;
                ELEM_SORT(arr[4], arr[5]) ; ELEM_SORT(arr[1], arr[4]) ; ELEM_SORT(arr[1], arr[3]) ;
                ELEM_SORT(arr[3], arr[4]) ; 
                return arr[3];
                break;

            case 6:
                ELEM_SORT(arr[0], arr[5]) ; ELEM_SORT(arr[1], arr[3]) ; ELEM_SORT(arr[2], arr[4]) ;
                ELEM_SORT(arr[1], arr[5]) ; ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[2], arr[5]) ;
                ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[0], arr[2]) ;
                ELEM_SORT(arr[2], arr[3]) ;
                return arr[2];
                break;

            case 5:
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[3], arr[4]) ; ELEM_SORT(arr[0], arr[3]) ;
                ELEM_SORT(arr[1], arr[4]) ; ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[2], arr[3]) ;
                ELEM_SORT(arr[1], arr[2]) ; 
                return arr[2];
                break;
        
            case 4:
                ELEM_SORT(arr[2], arr[3]) ; ELEM_SORT(arr[0], arr[3]) ; ELEM_SORT(arr[0], arr[1]) ;
                ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[0], arr[1]) ; 
                return arr[1];
                break;

            case 3:
                ELEM_SORT(arr[0], arr[1]) ; ELEM_SORT(arr[1], arr[2]) ; ELEM_SORT(arr[0], arr[1]) ;
                return arr[1];
                break;

            case 2:
            case 1:
                return arr[0];
                break;

            default:
                return 0;
                break;
        }
    }
    else
    {
        int low, high ;
        int median;
        int middle, ll, hh;

        low = 0 ; high = n-1 ; median = (low + high) / 2;
        for (;;) {
            if (high <= low) /* One element only */
                return arr[median] ;

            if (high == low + 1) {  /* Two elements only */
                if (arr[low] > arr[high])
                    ELEM_SWAP(arr[low], arr[high]) ;
                return arr[median] ;
            }

            /* Find median of low, middle and high items; swap into position low */
            middle = (low + high) / 2;
            if (arr[middle] > arr[high])    ELEM_SWAP(arr[middle], arr[high]) ;
            if (arr[low] > arr[high])       ELEM_SWAP(arr[low], arr[high]) ;
            if (arr[middle] > arr[low])     ELEM_SWAP(arr[middle], arr[low]) ;

            /* Swap low item (now in position middle) into position (low+1) */
            ELEM_SWAP(arr[middle], arr[low+1]) ;

            /* Nibble from each end towards middle, swapping items when stuck */
            ll = low + 1;
            hh = high;
            for (;;) {
                do ll++; while (arr[low] > arr[ll]) ;
                do hh--; while (arr[hh]  > arr[low]) ;

                if (hh < ll)
                    break;

                ELEM_SWAP(arr[ll], arr[hh]) ;
            }

            /* Swap middle item (in position low) back into correct position */
            ELEM_SWAP(arr[low], arr[hh]) ;

            /* Re-set active partition */
            if (hh <= median)
                low = ll;
            if (hh >= median)
                high = hh - 1;
        }
    }
}

#undef ELEM_SWAP
#undef ELEM_SORT
