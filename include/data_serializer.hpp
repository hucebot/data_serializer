#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <Eigen/Core>

namespace data_serializer {

    template <typename Map = std::unordered_map<std::string, Eigen::VectorXd>>
    class Writer {
    public:
        using data_t = Map;
        Writer(const std::string& filename) : _ofs(filename.c_str()) {}
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
        bool _header_written;
    };

    template <typename Map = std::unordered_map<std::string, Eigen::VectorXd>>
    class Reader {
    public:
        using data_t = Map;
        Reader(const std::string& filename) : _ifs(filename.c_str()) {}

        const std::vector<data_t>& read_all()
        {
            _read_header();
            while (true) {
                data_t d;
                for (size_t i = 0; i < _n_elems.size(); ++i) {
                    Eigen::VectorXd v(_n_elems[i]);
                    _ifs.read((char*)v.data(), sizeof(double) * _n_elems[i]);
                    if (_ifs.eof())
                        return _data;
                    d[_names[i]] = v;
                }
                _data.push_back(d);
            }
            return _data;
        }
        const std::vector<data_t>& data() const { return _data; }
        const std::vector<std::string>& names() const { return _names; }
        const std::vector<std::string>& n_elems() const { return _n_elems; }

        void write_csv(const std::string& fname, bool write_header = true, char delim = ';')
        {
            if (_names.empty())
                read_all(); // because of this, this write_csv cannot be const!
            std::ofstream ofs(fname.c_str());
            // header
            for (size_t i = 0; i < _names.size(); ++i)
                for (size_t j = 0; j < _n_elems[i]; ++j) {
                    ofs << _names[i] + "_" + std::to_string(j);
                    ofs << delim;
                }
            ofs << std::endl;

            // data
            for (auto& row : _data) {
                for (auto& col : row)
                    for (size_t i = 0; i < col.second.size(); ++i)
                        ofs << col.second[i] << delim;
                ofs << std::endl;
            }
        }

    protected:
        std::ifstream _ifs;
        std::vector<size_t> _n_elems;
        std::vector<std::string> _names;
        std::vector<data_t> _data;

        void _read_header()
        {
            size_t n_columns = read_size_t();
            _n_elems.resize(n_columns);
            _names.resize(n_columns);
            for (size_t i = 0; i < n_columns; ++i) {
                _n_elems[i] = read_size_t();
                size_t n = read_size_t();
                char s[n + 1];
                s[n] = 0;
                _ifs.read(s, n);
                _names[i] = std::string(s);
            }
        }
        size_t read_size_t()
        {
            size_t n;
            _ifs.read((char*)&n, sizeof(size_t));
            return n;
        }
    };

} // namespace data_serializer