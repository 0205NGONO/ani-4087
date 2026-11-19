#include <iostream>
#include <string>

struct Session {
    bool open = false;
    int begins = 0;
    int ends = 0;
    int refusals = 0;

    bool Wait() {
        if (open) {
            ++refusals;
            return false;
        }
        return true;
    }

    bool Begin() {
        if (open) {
            ++refusals;
            return false;
        }
        open = true;
        ++begins;
        return true;
    }

    bool End() {
        if (!open) {
            ++refusals;
            return false;
        }
        open = false;
        ++ends;
        return true;
    }
};

class GardeTrame {
public:
    explicit GardeTrame(Session &session) : session_(session) {}
    GardeTrame(const GardeTrame &) = delete;
    GardeTrame &operator=(const GardeTrame &) = delete;

    ~GardeTrame() {
        session_.End();
    }

private:
    Session &session_;
};

struct Result {
    int begins;
    int ends;
    int refusals;
    int images;
    bool open;
};

Result run(const std::string &paths, bool guarded) {
    Session session;
    int images = 0;
    bool stopped = false;

    for (char path : paths) {
        stopped = [&]() {
            const bool waitAccepted = session.Wait();
            const bool beginAccepted = session.Begin();
            const char effectivePath =
                !waitAccepted && (path == 'N' || path == 'C') ? 'V' : path;
            if (effectivePath == 'N' && beginAccepted) {
                ++images;
            }

            if (guarded) {
                if (beginAccepted) {
                    GardeTrame guard(session);
                    return effectivePath == 'R';
                }
            }

            if (effectivePath != 'C' && effectivePath != 'R') {
                session.End();
            }
            return effectivePath == 'R';
        }();

        if (stopped) {
            break;
        }
    }

    return {session.begins, session.ends, session.refusals, images, session.open};
}

void print(const char *label, const Result &result) {
    std::cout << label << " DEBUTS " << result.begins
              << " FINS " << result.ends
              << " REFUS " << result.refusals
              << " IMAGES " << result.images
              << " OUVERTE " << (result.open ? "OUI" : "NON") << '\n';
}

int main() {
    int frameCount = 0;
    std::string paths;
    if (!(std::cin >> frameCount)) {
        return 0;
    }
    if (frameCount > 0) {
        std::cin >> paths;
    }

    print("SANS", run(paths, false));
    print("AVEC", run(paths, true));
    return 0;
}
