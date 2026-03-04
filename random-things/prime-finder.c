#include <stdint.h>
#include <stdio.h>

int main() {
    int var;

    scanf("%d", &var);
    if (var % 2 != 0) {
        // done this way because we're checking up until the middle, as that's
        // the biggest factor * 2, which we check with the condition already.
        // There's a better way of handling this, and is to check up until
        // square var. This because the factor square var (if exist as a factor)
        // is the one that marks the tendency of factors to start being the
        // other side of the ones we already found by this point (or didn't, in
        // which case, is prime) I won't code it because it was not my idea haha
        for (int i = 2; i < var / 2; i++) {
            if (var % i == 0) {
                printf("not prime\n");
                return 0;
            }
        }
    } else if (var > 2) {
        printf("not prime\n");
        return 0;
    }

    printf("prime\n");
    return 0;
}
