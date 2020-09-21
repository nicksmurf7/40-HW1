# Comp40 Hw1: FilesnPix
Nick Murphy(nmurph03) and Reed Kass-Mullet(rkass01)



Total Time spent: Too much

Acknowledgements:
Michael Kotlik, Isabela Urdahl, Daniel Alderman, Mateo Hadeshian, Jack Burns, Miko Hadeshian, Greg Aloupis


Correctly implemented aspects of our programs:
    - Brightness works! We hope! Even though u can't see it on Git!
    - Simlines successfully reads in files, creates database of matches, and identifies matches
    - Simlines has memory leaks
    - Simlines can handle large lines
    - Simlines can handle multiple files
    - Simlines correctly sanitizes lines as per the COMP40 filesnpix spec

Incorrectly implemented aspects of our programs:
    - Readaline doesn't apply "If readaline is called when there are no more lines to be read, 
        it sets *datapp to NULL and returns 0." as per the COMP40 filesnpix spec
