// 
//  Public domain.
// 
// 
//  utils.c by Rafael Senties Martinelli.
// 
//  To the extent possible under law, the person who associated CC0 with
//  utils.c has waived all copyright and related or neighboring rights
//  to utils.c.
// 
//  You should have received a copy of the CC0 legalcode along with this
//  work.  If not, see <https://creativecommons.org/publicdomain/zero/1.0/>.

#include <vlc/vlc.h>

int digits_nb(libvlc_time_t number) {

    int digits = 1;

    if (number < 0){
        number = (number == INT64_MIN) ? INT64_MAX: -number;
    };

    while (number > 9) {
        number /= 10;
        digits++;
    }

    return digits;

};
