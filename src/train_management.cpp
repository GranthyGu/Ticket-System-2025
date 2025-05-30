// Created by GranthyGu on 2025/5/16

#include "train_management.hpp"

train_id::train_id() {}
train_id::train_id(const std::string& str) {
    for (int i = 0; i < str.size(); i++) {
        id[i] = str[i];
    }
}
train_id::train_id(const train_id& other) {
    for (int i = 0; i < 20; i++) {
        id[i] = other.id[i];
    }
}
train_id& train_id::operator=(const train_id& other) {
    for (int i = 0; i < 20; i++) {
        id[i] = other.id[i];
    }
    return *this;
}
bool train_id::operator<(const train_id& other) const {
    return strcmp(id,other.id) < 0;
}
bool train_id::operator>(const train_id& other) const {
    return strcmp(id,other.id) > 0;
}
bool train_id::operator==(const train_id& other) const {
    return strcmp(id,other.id) == 0;
}

station::station() {}
station::station(const std::string& str, const std::string& str_, const Time& t, const Time& t_) {
    for (int i = 0; i < str.size(); i++) {
        station_name[i] = str[i];
    }
    train_id id_(str_);
    id = id_;
    time_arrival = t;
    time_leave = t_;
}
station::station(const std::string& str, const train_id& id_, const Time& t, const Time& t_) {
    for (int i = 0; i < str.size(); i++) {
        station_name[i] = str[i];
    }
    id = id_;
    time_arrival = t;
    time_leave = t_;
}
station::station(const station& other) {
    for (int i = 0; i < 30; i++) {
        station_name[i] = other.station_name[i];
    }
    id = other.id;
    time_arrival = other.time_arrival;
    time_leave = other.time_leave;
}
station& station::operator=(const station& other) {
    for (int i = 0; i < 30; i++) {
        station_name[i] = other.station_name[i];
    }
    id = other.id;
    time_arrival = other.time_arrival;
    time_leave = other.time_leave;
    return *this;
}
bool station::operator<(const station& other) const {
    int res = strcmp(station_name, other.station_name);
    if (res != 0) {
        return res < 0;
    }
    return id < other.id;
}
bool station::operator>(const station& other) const {
    int res = strcmp(station_name, other.station_name);
    if (res != 0) {
        return res > 0;
    }
    return id > other.id;
}
bool station::operator==(const station& other) const {
    int res = strcmp(station_name, other.station_name);
    if (res != 0) {
        return false;
    }
    return id == other.id;
}

information::information() {}
information::information(const std::string& num, const std::string& type_, const std::string& stations_,
                         const std::string& sale_date, const std::string& price) {
    station_num = std::stoi(num);
    type = type_[0];
    std::string begin, end;
    for (int i = 0; i < 5; i++) {
        begin += sale_date[i];
    }
    for (int i = 6; i < 11; i++) {
        end += sale_date[i];
    }
    date begin_(begin), end_(end);
    sale_date_begin = begin_;
    sale_date_end = end_;
    std::string temp;
    int flag = 0;
    for (int i = 0; i < stations_.size(); i++) {
        if (stations_[i] != '|') {
            temp += stations_[i];
        } else {
            for (int j = 0; j < temp.size(); j++) {
                stations[flag][j] = temp[j];
            }
            flag++;
            temp.clear();
        }
    }
    for (int j = 0; j < temp.size(); j++) {
        stations[flag][j] = temp[j];
    }
    std::string temp_;
    sjtu::vector<int> prices_;
    for (int i = 0; i < price.size(); i++) {
        if (price[i] != '|') {
            temp_ += price[i];
        } else {
            prices_.push_back(std::stoi(temp_));
            temp_.clear();
        }
    }
    prices_.push_back(std::stoi(temp_));
    for (int i = 1; i <= prices_.size(); i++) {
        prices[i] = prices[i - 1] + prices_[i - 1];
    }
}
information::information(const information& other) {
    station_num = other.station_num;
    type = other.type;
    sale_date_begin = other.sale_date_begin;
    sale_date_end = other.sale_date_end;
    for (int i = 0; i < 31; i++) {
        prices[i] = other.prices[i];
    }
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            stations[i][j] = other.stations[i][j];
        }
    }
}
information& information::operator=(const information& other) {
    station_num = other.station_num;
    type = other.type;
    sale_date_begin = other.sale_date_begin;
    sale_date_end = other.sale_date_end;
    for (int i = 0; i < 30; i++) {
        prices[i] = other.prices[i];
    }
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            stations[i][j] = other.stations[i][j];
        }
    }
    return *this;
}
sjtu::vector<std::string> information::get_stations() {
    sjtu::vector<std::string> v;
    for (int i = 0; i < station_num; i++) {
        std::string temp(stations[i]);
        v.push_back(temp);
    }
    return v;
}
void information::write_to_file(std::fstream& File, long pos) {
    if (!File) {return;}
    File.seekp(pos);
    File.write(reinterpret_cast<char*> (this), sizeof(*this));
}
void information::read_from_file(std::fstream& File, long pos) {
    if (!File) {return;}
    File.seekg(pos);
    File.read(reinterpret_cast<char*> (this), sizeof(*this));
}


train_information::train_information() {}
train_information::train_information(const std::string& str1, const std::string& str2, const std::string& str3, const std::string& str4) {
    int num = std::stoi(str1);
    seat_num_initial = num;
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 92; j++) {
            seat_num[i][j] = num;
        }
    }
    Time start(str2);
    start_time = start;
    sjtu::vector<int> arrive, leave;
    std::string temp;
    for (int i = 0; i < str3.size(); i++) {
        if (str3[i] != '|') {
            temp += str3[i];
        } else {
            arrive.push_back(std::stoi(temp));
            temp.clear();
        }
    }
    arrive.push_back(std::stoi(temp));
    temp.clear();
    if (arrive.size() > 1) {
        for (int i = 0; i < str4.size(); i++) {
            if (str4[i] != '|') {
                temp += str4[i];
            } else {
                leave.push_back(std::stoi(temp));
                temp.clear();
            }
        }
        leave.push_back(std::stoi(temp));
    }
    arriving_time[0] = start;
    leaving_time[0] = start;
    for (int i = 1; i < arrive.size(); i++) {
        start.add_minute(arrive[i - 1]);
        arriving_time[i] = start;
        start.add_minute(leave[i - 1]);
        leaving_time[i] = start;
    }
    start.add_minute(arrive[arrive.size() - 1]);
    arriving_time[arrive.size()] = start;
}
train_information::train_information(const train_information& other) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 92; j++) {
            seat_num[i][j] = other.seat_num[i][j];
        }
        arriving_time[i] = other.arriving_time[i];
        leaving_time[i] = other.leaving_time[i];
    }
    start_time = other.start_time;
    seat_num_initial = other.seat_num_initial;
    released = other.released;
}
train_information& train_information::operator=(const train_information& other) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 92; j++) {
            seat_num[i][j] = other.seat_num[i][j];
        }
        arriving_time[i] = other.arriving_time[i];
        leaving_time[i] = other.leaving_time[i];
    }
    start_time = other.start_time;
    seat_num_initial = other.seat_num_initial;
    released = other.released;
    return *this;
}
void train_information::write_to_file(std::fstream& File, long pos) {
    if (!File) {return;}
    File.seekp(pos);
    File.write(reinterpret_cast<char*> (this), sizeof(*this));
}
void train_information::read_from_file(std::fstream& File, long pos) {
    if (!File) {return;}
    File.seekg(pos);
    File.read(reinterpret_cast<char*> (this), sizeof(*this));
}

train_management::train_management() {
    basic_information.set_file_name("train_information_1");
    released_station_train_id_list.set_file_name("train_released_list");
    advanced_information.set_file_name("train_information_2");
    File.open("file_for_train_information", std::ios::in | std::ios::out | std::ios::binary);
    File_.open("file_for_train_information_", std::ios::in | std::ios::out | std::ios::binary);
    if (!File) {
        File.clear();
        File.open("file_for_train_information", std::ios::out | std::ios::binary);
        File.close();
        File.clear();
        File.open("file_for_train_information", std::ios::in | std::ios::out | std::ios::binary);
    }
    if (!File_) {
        File_.clear();
        File_.open("file_for_train_information_", std::ios::out | std::ios::binary);
        File_.close();
        File_.clear();
        File_.open("file_for_train_information_", std::ios::in | std::ios::out | std::ios::binary);
    }
}
void train_management::add_train(const token_scanner& ts) {
    std::string trainID, station_num_, seat_num_, stations_, prices_, start_time_,
                travel_times_, stop_over_times_, sale_date_, type_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'i') {
            trainID= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'n') {
            station_num_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'm') {
            seat_num_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 's') {
            stations_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            prices_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'x') {
            start_time_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 't') {
            travel_times_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'o') {
            stop_over_times_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            sale_date_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'y') {
            type_ = ts.key_argument[i].second;
        }
    }
    information info(station_num_, type_, stations_, sale_date_, prices_);
    train_information info_(seat_num_, start_time_, travel_times_, stop_over_times_);
    train_id id(trainID);
    sjtu::vector<std::pair<train_id, long> > v = basic_information.find(id, id);
    if (!v.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    File_.seekp(0, std::ios::end);
    long pos_ = File_.tellp();
    info.write_to_file(File_, pos_);
    basic_information.insert(id, pos_);
    File.seekp(0, std::ios::end);
    long pos = File.tellp();
    info_.write_to_file(File, pos);
    advanced_information.insert(id, pos);
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void train_management::delete_train(const token_scanner& ts) {
    std::string ID = ts.key_argument[0].second;
    train_id id(ID);
    sjtu::vector<std::pair<train_id, long> > v = advanced_information.find(id, id);
    train_information info_;
    info_.read_from_file(File, v[0].second);
    if (v.empty() || info_.released == 1) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    basic_information.remove(id);
    advanced_information.remove(id);
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void train_management::release_train(const token_scanner& ts) {
    std::string ID = ts.key_argument[0].second;
    train_id id(ID);
    sjtu::vector<std::pair<train_id, long> > v = advanced_information.find(id, id);
    sjtu::vector<std::pair<train_id, long> > v_ = basic_information.find(id, id);
    train_information info_t;
    info_t.read_from_file(File, v[0].second);
    if (v.empty() || info_t.released == 1) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    information info;
    info.read_from_file(File_, v_[0].second);
    info_t.released = 1;
    info_t.write_to_file(File, v[0].second);
    sjtu::vector<std::string> station_info = info.get_stations();
    for (int i = 0; i < station_info.size(); i++) {
        station station_(station_info[i], ID, info_t.arriving_time[i], info_t.leaving_time[i]);
        released_station_train_id_list.insert(station_, std::make_pair(i, info.prices[i]));
    }
    std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
    return;
}
void train_management::query_train(const token_scanner& ts) {
    std::string ID, date_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 'i') {
            ID= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            date_ = ts.key_argument[i].second;
        }
    }
    train_id id(ID);
    date date_of_train(date_);
    sjtu::vector<std::pair<train_id, long> > v = advanced_information.find(id, id);
    sjtu::vector<std::pair<train_id, long> > v_ = basic_information.find(id, id);
    train_information info_t;
    info_t.read_from_file(File, v[0].second);
    if (v.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    information info;
    info.read_from_file(File_, v_[0].second);
    if (info.sale_date_begin > date_of_train || info.sale_date_end < date_of_train) {
        std::cout << '[' << ts.time << ']' << ' ' << -1 << std::endl;
        return;
    }
    sjtu::vector<std::string> stations_info = info.get_stations();
    std::cout << '[' << ts.time << ']' << ' ' << ID << ' ' << info.type << std::endl;
    for (int i = 0; i < info.station_num; i++) {
        std::cout << stations_info[i] << ' ';
        if (i == 0) {
            std::cout << "xx-xx xx:xx -> ";
        } else {
            Time t = info_t.arriving_time[i];
            date day_ = date_of_train;
            day_.add_day(t.day);
            std::cout << day_.to_string() << ' ' << t.to_string() << " -> ";
        }
        if (i == info.station_num - 1) {
            std::cout << "xx-xx xx:xx ";
        } else {
            Time t = info_t.leaving_time[i];
            date day_ = date_of_train;
            day_.add_day(t.day);
            std::cout << day_.to_string() << ' ' << t.to_string() << ' ';
        }
        std::cout << info.prices[i] << ' ';
        int delta = date_of_train.delta_day();
        if (i == info.station_num - 1) {
            std::cout << 'x' << std::endl;
        } else {
            std::cout << info_t.seat_num[i][delta] << std::endl;
        }
    }
}

bool compare_time(const temp &a, const temp &b) {
    if (a.time_ != b.time_) {
        return a.time_ < b.time_;
    }
    return a.begin.id < b.begin.id;
}
bool compare_price(const temp &a, const temp &b) {
    if (a.price != b.price) {
        return a.price < b.price;
    }
    return a.begin.id < b.begin.id;
}
bool compare_time_transfer(const std::pair<std::pair<temp, temp>, int>& a, const std::pair<std::pair<temp, temp>, int>& b) {
    int a_time = 1440 * a.second + a.first.first.time_ + a.first.second.time_ + (a.first.second.begin.time_leave.hour - a.first.first.end.time_arrival.hour) * 60 
                 + (a.first.second.begin.time_leave.minute - a.first.first.end.time_arrival.minute); 
    int b_time = 1440 * b.second + b.first.first.time_ + b.first.second.time_ + (b.first.second.begin.time_leave.hour - b.first.first.end.time_arrival.hour) * 60 
                 + (b.first.second.begin.time_leave.minute - b.first.first.end.time_arrival.minute); 
    int a_price = a.first.first.price + a.first.second.price;
    int b_price = b.first.first.price + b.first.second.price;
    if (a_time != b_time) {
        return a_time < b_time;
    }
    if (a_price != b_price) {
        return a_price < b_price;
    }
    if (!(a.first.first.begin.id == b.first.first.begin.id)) {
        return a.first.first.begin.id < b.first.first.begin.id;
    }
    return a.first.second.begin.id < b.first.second.begin.id;
}
bool compare_price_transfer(const std::pair<std::pair<temp, temp>, int>& a, const std::pair<std::pair<temp, temp>, int> b) {
    int a_time = 1440 * a.second + a.first.first.time_ + a.first.second.time_ + (a.first.second.begin.time_leave.hour - a.first.first.end.time_arrival.hour) * 60 
                 + (a.first.second.begin.time_leave.minute - a.first.first.end.time_arrival.minute); 
    int b_time = 1440 * b.second + b.first.first.time_ + b.first.second.time_ + (b.first.second.begin.time_leave.hour - b.first.first.end.time_arrival.hour) * 60 
                 + (b.first.second.begin.time_leave.minute - b.first.first.end.time_arrival.minute); 
    int a_price = a.first.first.price + a.first.second.price;
    int b_price = b.first.first.price + b.first.second.price;
    if (a_price != b_price) {
        return a_price < b_price;
    }
    if (a_time != b_time) {
        return a_time < b_time;
    }
    if (!(a.first.first.begin.id == b.first.first.begin.id)) {
        return a.first.first.begin.id < b.first.first.begin.id;
    }
    return a.first.second.begin.id < b.first.second.begin.id;
}
template <typename Compare, class T>
void quick_sort(sjtu::vector<T> &v, int left, int right, Compare cmp) {
    if (left >= right) return;
    T pivot = v[left];
    int i = left - 1, j = right + 1;
    while (true) {
        do { i++; } while (cmp(v[i],pivot));
        do { j--; } while (cmp(pivot,v[j]));
        if (i >= j) break;
        std::swap(v[i], v[j]);
    }
    quick_sort(v, left, j,cmp);
    quick_sort(v, j + 1, right,cmp);
}
void sort_by_time(sjtu::vector<temp> &v) {
    quick_sort(v, 0, v.size() - 1, compare_time);
}
void sort_by_price(sjtu::vector<temp> &v) {
    quick_sort(v, 0, v.size() - 1, compare_price);
}
void sort_by_time_transfer(sjtu::vector<std::pair<std::pair<temp, temp>, int> > &v) {
    quick_sort(v, 0, v.size() - 1, compare_time_transfer);
}
void sort_by_price_transfer(sjtu::vector<std::pair<std::pair<temp, temp>, int> > &v) {
    quick_sort(v, 0, v.size() - 1, compare_price_transfer);
}

sjtu::vector<temp> train_management::query_ticket_(const std::string& start, const std::string& end, const date& date_) {
    std::string min_id, max_id;
    for (int i = 0; i < 20; i++) {
        min_id += '\0';
        max_id += '~';
    }
    Time t;
    station minimal_start(start, min_id, t, t), maximal_start(start, max_id, t, t);
    station minimal_end(end, min_id, t, t), maximal_end(end, max_id, t, t);
    sjtu::vector<std::pair<station, std::pair<int, int> > > start_train = released_station_train_id_list.find(minimal_start, maximal_start);
    sjtu::vector<std::pair<station, std::pair<int, int> > > end_train = released_station_train_id_list.find(minimal_end, maximal_end);
    sjtu::vector<temp> train_satisfied;
    int ii = 0, jj = 0;
    sjtu::vector<std::pair<int, int>> satisfy;
    while (ii < start_train.size() && jj < end_train.size()) {
        if (start_train[ii].first.id < end_train[jj].first.id) {
            ii++;
        } else if (start_train[ii].first.id > end_train[jj].first.id) {
            jj++;
        } else if (start_train[ii].second.first < end_train[jj].second.first) {
            satisfy.push_back({ii, jj});
            ii++;
            jj++;
        } else {
            ii++;
            jj++;
        }
    }
    for (auto k : satisfy) {
        int i = k.first, j = k.second;
        station train = start_train[i].first;
        station train_end = end_train[j].first;
        date d(date_);
        d.minus_day(train.time_leave.day);
        sjtu::vector<std::pair<train_id, long> > v_ = basic_information.find(train.id, train.id);
        information info;
        info.read_from_file(File_, v_[0].second);
        if (info.sale_date_begin > d || info.sale_date_end < d) {
            continue;
        }
        int price = end_train[j].second.second - start_train[i].second.second;
        int t = train_end.time_arrival - train.time_leave;
        int index_begin = start_train[i].second.first;
        int index_end = end_train[j].second.first;
        train_satisfied.push_back({train, train_end, t, price, index_begin, index_end});
    }
    return train_satisfied;
}
void train_management::query_ticket(const token_scanner& ts) {
    std::string start, end, date_, sort_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 's') {
            start= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            date_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 't') {
            end = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            sort_ = ts.key_argument[i].second;
        }
    }
    date d(date_);
    date minimal_d("06-01");
    date maximal_d("08-31");
    if (d < minimal_d || d > maximal_d) {
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
    sjtu::vector<temp> train_satisfied = query_ticket_(start, end, d);
    if (train_satisfied.empty()) {
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
    if (sort_ == "cost") {
        sort_by_price(train_satisfied);
    } else if (sort_ == "time") {
        sort_by_time(train_satisfied);
    }
    std::cout << '[' << ts.time << ']' << ' ' << train_satisfied.size() << std::endl;
    for (int i = 0; i < train_satisfied.size(); i++) {
        std::cout << train_satisfied[i].begin.id.id << ' ' << train_satisfied[i].begin.station_name << ' ';
        date day(date_);
        date day_ = day;
        day_.minus_day(train_satisfied[i].begin.time_leave.day);
        std::cout << day.to_string() << ' ' << train_satisfied[i].begin.time_leave.to_string() 
                  << " -> " << train_satisfied[i].end.station_name << ' ';
        day.add_day(train_satisfied[i].end.time_arrival.day - train_satisfied[i].begin.time_leave.day);
        std::cout << day.to_string() << ' ' << train_satisfied[i].end.time_arrival.to_string() << ' ' << train_satisfied[i].price << ' ';
        sjtu::vector<std::pair<train_id, long> > v = advanced_information.find(train_satisfied[i].begin.id, train_satisfied[i].begin.id);
        train_information info_;
        info_.read_from_file(File, v[0].second);
        int seat_ = 1e9;
        int delta = day_.delta_day();
        for (int j = train_satisfied[i].index_begin; j < train_satisfied[i].index_end; j++) {
            seat_ = std::min(seat_, info_.seat_num[j][delta]);
        }
        std::cout << seat_ << std::endl;
    }
}
void train_management::query_transfer(const token_scanner& ts) {
    std::string start, end, date_, sort_;
    for (int i = 0; i < ts.key_argument.size(); i++) {
        if (ts.key_argument[i].first == 's') {
            start= ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'd') {
            date_ = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 't') {
            end = ts.key_argument[i].second;
        } else if (ts.key_argument[i].first == 'p') {
            sort_ = ts.key_argument[i].second;
        }
    }
    date d(date_);
    date minimal_d("06-01");
    date maximal_d("08-31");
    if (d < minimal_d || d > maximal_d) {
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
    sjtu::vector<std::pair<std::pair<temp, temp>, int> > train_satisfied;
    std::string min_id, max_id;
    for (int i = 0; i < 20; i++) {
        min_id += '\0';
        max_id += '~';
    }
    station minimal_start(start, min_id, Time(), Time()), maximal_start(start, max_id, Time(), Time());
    station minimal_end(end, min_id, Time(), Time()), maximal_end(end, max_id, Time(), Time());
    sjtu::vector<std::pair<station, std::pair<int, int> > > start_train = released_station_train_id_list.find(minimal_start, maximal_start);
    sjtu::vector<std::pair<station, std::pair<int, int> > > end_train = released_station_train_id_list.find(minimal_end, maximal_end);
    for (auto i : start_train) {
        sjtu::vector<std::pair<train_id, long>> temp_ = basic_information.find(i.first.id, i.first.id);
        train_information info_;
        info_.read_from_file(File, advanced_information.find(i.first.id, i.first.id)[0].second);
        information info_t;
        info_t.read_from_file(File_, temp_[0].second);
        sjtu::vector<std::string> info = info_t.get_stations();
        int index = i.second.first;
        date initial_date = d;
        initial_date.minus_day(info_.leaving_time[index].day);
        if (initial_date < info_t.sale_date_begin || initial_date > info_t.sale_date_end){
            continue;
        }
        for (auto j : end_train) {
            if (j.first.id == i.first.id) {
                continue;
            }
            train_information info_end;
            info_end.read_from_file(File, advanced_information.find(j.first.id, j.first.id)[0].second);
            information info_t_end;
            info_t_end.read_from_file(File_, basic_information.find(j.first.id, j.first.id)[0].second);
            sjtu::vector<std::string> in_ = info_t_end.get_stations();
            for (int k = j.second.first - 1; k >= 0; k--) {
                for (int l = i.second.first + 1; l < info_t.station_num; l++) {
                    if (info[l] == in_[k]) {
                        Time arr = info_.arriving_time[l];
                        Time lea = info_end.leaving_time[k];
                        date ddd = initial_date;
                        ddd.add_day(arr.day);
                        ddd.minus_day(lea.day);
                        if (info_t_end.sale_date_end < ddd || (info_t_end.sale_date_end == ddd && (lea.hour < arr.hour || (lea.hour == arr.hour && lea.minute < arr.minute)))) {
                            continue;
                        }
                        int day_ = 0;
                        if (info_t_end.sale_date_begin > ddd) {
                            day_ = info_t_end.sale_date_begin.delta_day() - ddd.delta_day();
                        } else {
                            if ((lea.hour < arr.hour || (lea.hour == arr.hour && lea.minute < arr.minute))) {
                                day_ = 1;
                            }
                        }
                        station end1(info[l], i.first.id, arr, info_.leaving_time[l]);
                        station start2(in_[k], j.first.id, info_end.arriving_time[k], lea);
                        temp fir = {i.first, end1, arr - i.first.time_leave, info_t.prices[l] - info_t.prices[i.second.first], i.second.first, l};
                        temp en = {start2, j.first, j.first.time_arrival - lea, j.second.second - info_t_end.prices[k], k, j.second.first};
                        train_satisfied.push_back({{fir, en}, day_});
                    } else {
                        continue;
                    }
                }
            }
        }
        
    }
    if (train_satisfied.size() == 0) {
        std::cout << '[' << ts.time << ']' << ' ' << 0 << std::endl;
        return;
    }
    if (sort_ == "time") {
        sort_by_time_transfer(train_satisfied);
    } else if (sort_ == "cost") {
        sort_by_price_transfer(train_satisfied);
    }
    std::cout << '[' << ts.time << "] ";
    std::cout << train_satisfied[0].first.first.begin.id.id << ' ' << train_satisfied[0].first.first.begin.station_name << ' ';
    date day(date_);
    date day_(date_);
    day_.minus_day(train_satisfied[0].first.first.begin.time_leave.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.first.begin.time_leave.to_string() 
                << " -> " << train_satisfied[0].first.first.end.station_name << ' ';
    day.add_day(train_satisfied[0].first.first.end.time_arrival.day - train_satisfied[0].first.first.begin.time_leave.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.first.end.time_arrival.to_string() << ' ' << train_satisfied[0].first.first.price << ' ';
    train_information info_t;
    info_t.read_from_file(File, advanced_information.find(train_satisfied[0].first.first.begin.id, train_satisfied[0].first.first.begin.id)[0].second);
    int seat_ = 1e9;
    int delta = day_.delta_day();
    for (int j = train_satisfied[0].first.first.index_begin; j < train_satisfied[0].first.first.index_end; j++) {
        seat_ = std::min(seat_, info_t.seat_num[j][delta]);
    }
    std::cout << seat_ << std::endl;
    std::cout << train_satisfied[0].first.second.begin.id.id << ' ' << train_satisfied[0].first.second.begin.station_name << ' ';
    day.add_day(train_satisfied[0].second);
    day_ = day;
    day_.minus_day(train_satisfied[0].first.second.begin.time_leave.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.second.begin.time_leave.to_string() 
                << " -> " << train_satisfied[0].first.second.end.station_name << ' ';
    day.add_day(train_satisfied[0].first.second.end.time_arrival.day - train_satisfied[0].first.second.begin.time_leave.day);
    std::cout << day.to_string() << ' ' << train_satisfied[0].first.second.end.time_arrival.to_string() << ' ' << train_satisfied[0].first.second.price << ' ';
    train_information info_tt;
    info_tt.read_from_file(File, advanced_information.find(train_satisfied[0].first.second.begin.id, train_satisfied[0].first.second.begin.id)[0].second);
    seat_ = 1e9;
    delta = day_.delta_day();
    for (int j = train_satisfied[0].first.second.index_begin; j < train_satisfied[0].first.second.index_end; j++) {
        seat_ = std::min(seat_, info_tt.seat_num[j][delta]);
    }
    std::cout << seat_ << std::endl;
    return;
}
std::pair<date, int> train_management::query_buy(const std::string& start, const std::string& end, date date_, const train_id& id, int num, bool flag) {
    Time t;
    station start_(start, id, t, t);
    station end_(end, id, t, t);
    sjtu::vector<std::pair<station, std::pair<int, int> > > start_train = released_station_train_id_list.find(start_, start_);
    sjtu::vector<std::pair<station, std::pair<int, int> > > end_train = released_station_train_id_list.find(end_, end_);
    if (start_train.empty() || end_train.empty()) {
        return {date(), -1};
    }
    sjtu::vector<temp> train_satisfied;
    station train_start = start_train[0].first;
    station train_end = end_train[0].first;
    if (start_train[0].second.first >= end_train[0].second.first) {
        return {date(), -1};
    }
    if (flag) date_.minus_day(train_start.time_leave.day);
    sjtu::vector<std::pair<train_id, long> > v_ = basic_information.find(id, id);
    information info;
    info.read_from_file(File_, v_[0].second);
    if (info.sale_date_begin > date_ || info.sale_date_end < date_) {
        return {date(), -1};
    }
    int delta_date = date_.delta_day();
    train_information info_;
    long pos = advanced_information.find(id, id)[0].second;
    info_.read_from_file(File, pos);
    int minimal = 1e9;
    if (info_.seat_num_initial < num) {
        return {date(), -1};
    }
    for (int i = start_train[0].second.first; i < end_train[0].second.first; i++) {
        minimal = std::min(minimal, info_.seat_num[i][delta_date]);
    }
    if (minimal >= num){
        for (int i = start_train[0].second.first; i < end_train[0].second.first; i++) {
            info_.seat_num[i][delta_date] -= num;
        }
        info_.write_to_file(File, pos);
        int price_ = end_train[0].second.second - start_train[0].second.second;
        return {date_, price_ * num};
    } else {
        return {date_, 0};
    }
}
std::pair<Time, Time> train_management::query_time(const train_id& train, const std::string& station_) {
    sjtu::vector<std::pair<train_id, long> > result_ = basic_information.find(train, train);
    information info;
    info.read_from_file(File_, result_[0].second);
    train_information info_;
    long pos = advanced_information.find(train, train)[0].second;
    info_.read_from_file(File, pos);
    sjtu::vector<std::string> stations = info.get_stations();
    int index;
    for (index = 0; index < stations.size(); index++) {
        if (stations[index] == station_) {
            break;
        }
    }
    Time time_arrive = info_.arriving_time[index];
    Time time_leave = info_.leaving_time[index];
    return {time_arrive, time_leave};
}
int train_management::query_price(const train_id& train, const std::string& begin, const std::string& end) {
    sjtu::vector<std::pair<train_id, long> > result_ = basic_information.find(train, train);
    information info;
    info.read_from_file(File_, result_[0].second);
    sjtu::vector<std::string> stations = info.get_stations();
    int index, index_;
    for (int i = 0; i < stations.size(); i++) {
        if (stations[i] == begin) {
            index = i;
        }
        if (stations[i] == end) {
            index_ = i;
        }
    }
    return info.prices[index_] - info.prices[index];
}
void train_management::refund_ticket(const train_id& train, std::string begin, std::string end, date day, int num) {
    sjtu::vector<std::pair<train_id, long> > result_ = basic_information.find(train, train);
    train_information info;
    long pos = advanced_information.find(train, train)[0].second;
    info.read_from_file(File, pos);
    information info_;
    long pos_ = result_[0].second;
    info_.read_from_file(File_, pos_);
    sjtu::vector<std::string> stations = info_.get_stations();
    int index, index_;
    for (int i = 0; i < stations.size(); i++) {
        if (stations[i] == begin) {
            index = i;
        }
        if (stations[i] == end) {
            index_ = i;
        }
    }
    day.minus_day(info.leaving_time[index].day);
    int delta = day.delta_day();
    for (int i = index; i < index_; i++) {
        info.seat_num[i][delta] += num;
    }
    info.write_to_file(File, pos);
}
void train_management::exit() {
    basic_information.~B_plus_tree();
    advanced_information.~B_plus_tree();
    released_station_train_id_list.~B_plus_tree();
}
void train_management::clear() {
    basic_information.clear();
    advanced_information.clear();
    released_station_train_id_list.clear();
}