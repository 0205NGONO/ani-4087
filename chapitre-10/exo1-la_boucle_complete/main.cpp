#include <iostream>
#include <string>
#include <vector>

int main() {
    int totalImages = 0;
    if (!(std::cin >> totalImages)) {
        return 0;
    }

    int correct = 0;
    int faulty = 0;

    for (int image = 1; image <= totalImages; ++image) {
        int callCount = 0;
        std::cin >> callCount;
        std::vector<std::string> calls(callCount);
        for (std::string &call : calls) {
            std::cin >> call;
        }

        bool waited = false;
        bool begun = false;
        bool locatedViews = false;
        bool borrowedLeft = false;
        bool borrowedRight = false;
        bool ended = false;
        int faultPosition = 0;
        std::string faultCode;

        for (int i = 0; i < callCount; ++i) {
            const std::string &call = calls[i];
            std::string code;

            if (ended) {
                code = "APRES_END";
            } else if (call == "WAIT") {
                if (begun) {
                    code = "WAIT_APRES_BEGIN";
                } else {
                    waited = true;
                }
            } else if (call == "BEGIN") {
                if (!waited) {
                    code = "BEGIN_SANS_WAIT";
                } else if (begun) {
                    code = "DOUBLE_BEGIN";
                } else {
                    begun = true;
                }
            } else if (call == "VUES") {
                if (!waited) {
                    code = "VUES_SANS_WAIT";
                } else {
                    locatedViews = true;
                }
            } else if (call == "PREND_G" || call == "PREND_D") {
                bool &borrowed = call == "PREND_G" ? borrowedLeft : borrowedRight;
                if (!begun) {
                    code = "PREND_SANS_BEGIN";
                } else if (!locatedViews) {
                    code = "PREND_SANS_VUES";
                } else if (borrowed) {
                    code = "DOUBLE_PREND";
                } else {
                    borrowed = true;
                }
            } else if (call == "LACHE_G" || call == "LACHE_D") {
                bool &borrowed = call == "LACHE_G" ? borrowedLeft : borrowedRight;
                if (!borrowed) {
                    code = "LACHE_SANS_PREND";
                } else {
                    borrowed = false;
                }
            } else if (call == "END") {
                if (!begun) {
                    code = "END_SANS_BEGIN";
                } else if (borrowedLeft || borrowedRight) {
                    code = "END_IMAGE_PRISE";
                } else {
                    ended = true;
                }
            }

            if (!code.empty()) {
                faultPosition = i + 1;
                faultCode = code;
                break;
            }
        }

        if (faultCode.empty() && !ended) {
            faultPosition = callCount + 1;
            faultCode = "SANS_END";
        }

        if (faultCode.empty()) {
            ++correct;
            std::cout << image << " OK\n";
        } else {
            ++faulty;
            std::cout << image << " FAUTE " << faultPosition << ' ' << faultCode << '\n';
        }
    }

    std::cout << "JUSTES " << correct << '\n'
              << "FAUTIVES " << faulty << '\n';
    return 0;
}
