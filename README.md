# data_serializer
A minimalistic C++ library for serializing Eigen::MatriXd data in a map (e.g., sensor data)

## Writing
```C++
#include <data_serializer/data_serializer.hpp>


int main() {
    
    std::unordered_map<std::string, Eigen::VectorXd> data;

    data["imu"] = Eigen::VectorXd::Ones(3);
    data["ft"] = Eigen::VectorXd::Zero(6);

    data_serializer::Writer<> writer("test.dat");
    writer.write(data);
    writer.write(data);
    writer.write(data);
    writer.flush();
    
    return 0;
}
```


## Reading
```C++

#include <data_serializer/data_serializer.hpp>


int main() {
   
    data_serializer::Reader<> reader("test.dat");
    std::cout<<"data size:"<<reader.data.size()<<std::endl;

    reader.write_csv("test.csv");
    
    return 0;
}
