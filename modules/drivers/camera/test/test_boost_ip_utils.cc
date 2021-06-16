#include <iostream>
#include <iterator>
#include <algorithm>

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>

#define UNUSED(x) do { (void)(x);} while (0)
int main()
{
  boost::system::error_code ec;
  std::string target_ip = "192.168.1.230";
  // auto ip_address = boost::asio::ip::make_address(target_ip, ec);
  auto ip_address = boost::asio::ip::address::from_string(target_ip, ec);

  UNUSED(ip_address);

  if (ec)
  {
    std::cout << "error" << std::endl;
  }
  else
  {
    std::cout << "no error" << std::endl;
    std::cout << "the address is: " << ip_address.to_string() << std::endl;
    std::cout << "the address is: " << ip_address.to_v4().to_ulong() << std::endl;
  }





}