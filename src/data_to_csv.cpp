#include <data_serializer/data_serializer.hpp>

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " in.dat in.csv" << std::endl;
        return 1;
    }
    std::cout << "Parsing " << argv[1] << "... ";
    std::cout.flush();
    data_serializer::Reader<> reader(argv[1]);
    std::cout << "done" << std::endl;

    std::cout << "Writing " << argv[2] << "... ";
    std::cout.flush();
    reader.write_csv(argv[2]);
    std::cout << "done" << std::endl;
    return 0;
}