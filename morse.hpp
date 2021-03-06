class Morse {
private:
    char *ascii = "abcdefghijklmnopqrstuvwxyz0123456789.,?'!/()&:;=+-_\"$@";
    char *morse[61] = {
     ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","-----",".----","..---","...--","....-",".....","-....","--...","---..","----.",".-.-.-","--..--","..--..",".----.","-.-.--","-..-.","-.--.","-.--.-",".-...","---...","-.-.-.","-...-",".-.-.","-....-","..--.-",".-..-.","...-..-",".--.-."
    };

public:
    char *fromAsc(char c);
};