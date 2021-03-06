#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

class KeyWatcher {
private:
    Display *display_name;
    char last_ongoing_key;

public:
    KeyWatcher();
    ~KeyWatcher();
    char check();
};

