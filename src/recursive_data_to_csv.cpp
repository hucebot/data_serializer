#include <data_serializer/data_serializer.hpp>
#include <filesystem>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " path_to_log_directory" << std::endl;
        return 1;
    }

    for (std::filesystem::recursive_directory_iterator i(argv[1]), end; i != end; ++i) {
        auto path_str = i->path().string();

        if (path_str.substr(path_str.size() - 4, path_str.size()) == ".dat") {

            auto out_path = path_str.substr(0, path_str.size() - 4) + ".csv";
           
            std::cout << "Parsing " << path_str << "... ";
            std::cout.flush();
            data_serializer::Reader<> reader(path_str);
            std::cout << "done" << std::endl;

            std::cout << "Writing " << out_path << "... ";
            std::cout.flush();
            reader.write_csv(out_path);
            std::cout << "done" << std::endl;
        }
    }

    return 0;
}