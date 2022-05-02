
#include <unordered_map>
#include <data_serializer.hpp>


int main() {
    {
    std::unordered_map<std::string, Eigen::VectorXd> data;

    data["imu"] = Eigen::VectorXd::Ones(3);
    data["ft"] = Eigen::VectorXd::Zero(6);


    data_serializer::Writer<> writer("test.dat");
    writer.write(data);
    writer.write(data);
    writer.write(data);
    writer.flush();
    }
    {
    data_serializer::Reader<> reader("test.dat");
    auto data = reader.read_all();
    reader.write_csv("test.csv");
    }
}