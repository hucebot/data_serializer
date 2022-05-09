#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

#include <Eigen/Core>

namespace data_serializer {

    template <typename Map = std::map<std::string, Eigen::VectorXd>>
    class Writer {
    public:
        using data_t = Map;
        Writer(const std::string& filename) : _ofs(filename.c_str())
        {
            if (!_ofs.good())
                throw std::runtime_error(std::string("Error when opening file ") + filename);
        }
        void write(const data_t& data)
        {
            if (!_header_written)
                _write_header(data);
            int k = 0;
            for (const auto& x : data)
                _ofs.write((char*)x.second.data(), x.second.size() * sizeof(double));
        }
        void flush() { _ofs.flush(); }

    protected:
        void _write_header(const data_t& data)
        {
            _write(data.size()); // number of "columns"
            for (const auto& x : data) {
                _write(x.second.size()); // number of elements in each "column"
                _write(x.first.size()); // size of the name
                _ofs.write(x.first.c_str(), x.first.size()); // column name
            }
            _header_written = true;
        }
        void _write(size_t x)
        {
            _ofs.write((char*)&x, sizeof(size_t));
        }
        std::ofstream _ofs;
        bool _header_written = false;
    };

    template <typename Map = std::map<std::string, Eigen::VectorXd>>
    class Reader {
    public:
        using data_t = Map;
        // vector of map
        std::vector<data_t> data;
        // number of items for each field
        std::vector<size_t> n_items;
        // name of each field
        std::vector<std::string> names;

        // parse a .dat file
        Reader(const std::string& filename) { _read_all(filename); }

        void write_csv(const std::string& fname, bool write_header = true, char delim = ';') const
        {
            std::ofstream ofs(fname.c_str());
            if (!ofs.good())
                throw std::runtime_error(std::string("Error when opening file ") + fname);

            // header
            for (size_t i = 0; i < names.size(); ++i)
                for (size_t j = 0; j < n_items[i]; ++j) {
                    ofs << names[i] + "_" + std::to_string(j);
                    ofs << delim;
                }
            ofs << std::endl;

            // data
            for (auto& row : data) {
                for (auto& col : row)
                    for (size_t i = 0; i < col.second.size(); ++i)
                        ofs << col.second[i] << delim;
                ofs << std::endl;
            }
        }

    protected:
        void _read_all(const std::string& filename)
        {
            std::ifstream ifs(filename.c_str());
            if (!ifs.good())
                throw std::runtime_error(std::string("Error when opening file ") + filename);
            _read_header(ifs);
            while (true) {
                data_t d;
                for (size_t i = 0; i < n_items.size(); ++i) {
                    Eigen::VectorXd v(n_items[i]);
                    ifs.read((char*)v.data(), sizeof(double) * n_items[i]);
                    if (ifs.eof())
                        return;
                    d[names[i]] = v;
                }
                data.push_back(d);
            }
        }
        void _read_header(std::ifstream& ifs)
        {
            size_t n_columns = read_size_t(ifs);
            n_items.resize(n_columns);
            names.resize(n_columns);
            for (size_t i = 0; i < n_columns; ++i) {
                n_items[i] = read_size_t(ifs);
                size_t n = read_size_t(ifs);
                char s[n + 1];
                s[n] = 0;
                ifs.read(s, n);
                names[i] = std::string(s);
            }
        }
        size_t read_size_t(std::ifstream& ifs)
        {
            size_t n;
            ifs.read((char*)&n, sizeof(size_t));
            return n;
        }
    };

} // namespace data_serializer