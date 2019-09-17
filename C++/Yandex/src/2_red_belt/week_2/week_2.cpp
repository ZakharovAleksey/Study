#include "week_2.h"

namespace week_2
{

void HM::Book(const string& hotel_name, int64_t cur_time, int32_t client_id,
              uint16_t room_count)
{
  // If client do not reserve any room -> do nothing
  if (room_count == 0)
  {
    return;
  } else
  {
    book_queries_[cur_id].cur_time = cur_time;
    book_queries_[cur_id].client_id = client_id;
    book_queries_[cur_id].room_count = room_count;
    book_queries_[cur_id].hotel_name = hotel_name;

    ++cur_id;
  }

  auto it = clients_numb_.find(hotel_name);
  if (it == end(clients_numb_))
  {
    clients_numb_[hotel_name][client_id] = 1u;
  } else
  {
    ++it->second[client_id];
  }

  rooms_numb_[hotel_name] += static_cast<uint32_t>(room_count);

  int64_t last_time = cur_time - DAY_TIME;
  while (book_queries_[start_id].cur_time <= last_time)
  {
    // clients_numb_[book_queries_[start_id].hotel_name].erase(book_queries_[start_id].client_id);
    --clients_numb_[book_queries_[start_id].hotel_name]
                   [book_queries_[start_id].client_id];

    if (clients_numb_[book_queries_[start_id].hotel_name]
                     [book_queries_[start_id].client_id] == 0u)
    {
      clients_numb_[book_queries_[start_id].hotel_name].erase(
          book_queries_[start_id].client_id);
    }

    rooms_numb_[book_queries_[start_id].hotel_name] -=
        static_cast<uint32_t>(book_queries_[start_id].room_count);

    ++start_id;
  }
}

uint32_t HM::Clients(const string& hotel_name) const
{
  auto it = clients_numb_.find(hotel_name);

  if (it == end(clients_numb_) || clients_numb_.empty())
  {
    return 0u;
  }

  return static_cast<uint32_t>(it->second.size());
}

uint32_t HM::Rooms(const string& hotel_name) const
{
  auto it = rooms_numb_.find(hotel_name);
  if (it == end(rooms_numb_) || rooms_numb_.empty())
  {
    return 0u;
  }

  return it->second;
}

} // namespace week_2
