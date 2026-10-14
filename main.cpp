#include <algorithm>
#include <cpr/cpr.h>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

namespace {
#define RESET "\033[0m"
#define RED "\033[31m"  /* Red */
#define BLUE "\033[34m" /* Blue */

struct Repo {
        std::string _name;
        std::string _commit;
        int _stars;

        Repo(std::string name, const int &stars, std::string commit)
            : _name(std::move(name)), _stars(stars), _commit(std::move(commit)) {};
};

cpr::Response get_json(const std::string &endpoint) {
        cpr::Response r = cpr::Get(cpr::Url{endpoint}, cpr::Timeout(5000));
        if (r.status_code == 0) {
                throw std::runtime_error(r.error.message);
        } else if (r.status_code >= 400) {
                throw std::runtime_error("HTTP " + std::to_string(r.status_code) +
                                         " while fetching repositories");
        } else {
                std::cout << "Request took " << r.elapsed << std::endl;
        }
        return r;
}

std::vector<Repo> get_repos(const std::string &endpoint) {
        std::vector<Repo> repos;
        cpr::Response r = get_json(endpoint);
        auto json = json::parse(r.text);

        for (const auto &item : json) {
                Repo temp{item["name"], item["stargazers_count"], item["pushed_at"]};
                repos.push_back(temp);
        }
        return repos;
}

void sort_repos(char **argv, std::vector<Repo> &repos, const int &i) {
        switch (argv[i + 1][0]) {
        case 's':
                std::ranges::sort(repos.begin(), repos.end(),
                                  [](const auto &repo_a, const auto &repo_b) {
                                          return repo_a._stars > repo_b._stars;
                                  });
                break;

        case 'c': {
                std::cout << "COMMIT SORT" << std::endl;
                std::ranges::sort(repos.begin(), repos.end(),
                                  [](const auto &repo_a, const auto &repo_b) {
                                          return repo_a._commit > repo_b._commit;
                                  });
                ;
                break;
        }
        default:
                std::cout << "Wrong sort parameter. See repos -h for help" << std::endl;
        }
}
} // namespace

int main(const int argc, char **argv) {
        if (argc < 2 || argv[1][0] == '\0') {
                std::cerr << "Usage: " << argv[0] << " <username> [--sort stars|commit]"
                          << std::endl;
                return 1;
        }
        const std::string username = argv[1];

        const std::string url = "https://api.github.com/users/";
        const std::string endpoint = url + username + "/repos";
        std::vector<Repo> repos;
        try {
                repos = get_repos(endpoint);
        } catch (const std::exception &error) {
                std::cerr << "Error: " << error.what() << std::endl;
                return 1;
        }

        // FLAGS
        std::cout << argc << std::endl;
        if (argc > 2) {
                for (int i = 2; i < argc; ++i) {
                        if (std::strcmp(argv[i], "--sort") == 0) {
                                sort_repos(argv, repos, i);
                        }
                }
        }

        std::cout << BLUE << std::setw(28) << std::left;
        std::cout << std::left << "Name" << std::setw(28) << "Stars" << "Pushed at" << RESET
                  << std::endl;

        for (const auto &repo : repos) {
                // TODO: If name longer than 27 truncate
                std::cout << std::setw(28) << std::left;
                std::cout << (repo._name.size() > 27 ? repo._name.substr(24) + "..." : repo._name)
                          << std::setw(28) << repo._stars << repo._commit << std::endl;
        }
        return 0;
}
