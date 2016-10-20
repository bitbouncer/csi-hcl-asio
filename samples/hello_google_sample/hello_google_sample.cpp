#include <stdio.h>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <csi_hcl_asio/http_client.h>

void log_result(csi::http_client::call_context::handle h) {
  if (h->ok()) {
    size_t sz = h->rx_content_length();
    size_t kb_per_sec = 0;
    if (h->milliseconds() > 0) {
      kb_per_sec = sz / h->milliseconds();
    }
    BOOST_LOG_TRIVIAL(info) << "http get: " << h->uri() << " got " << h->rx_content_length() << " bytes, time=" << h->milliseconds() << " ms (" << kb_per_sec << " KB/s)";
  } else if (!h->transport_result()) {
    BOOST_LOG_TRIVIAL(error) << "transport failed";
  } else {
    BOOST_LOG_TRIVIAL(error) << "http get: " << h->uri() << " HTTPRES = " << h->http_result();
  }
}

#define NR_OF_URI 2
std::string uris[NR_OF_URI] =
{
  "http://google.com",
  "http://dn.se"
};

int main(int argc, char **argv) {
  boost::asio::io_service io_service;
  auto keepalive_work = std::make_unique<boost::asio::io_service::work>(io_service);
  boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
  curl_global_init(CURL_GLOBAL_NOTHING); /* minimal */
  csi::http_client handler(io_service);
  /* enter io_service run loop */
  std::thread asio_thread([&] { io_service.run(); });

  for (int i = 0; i != NR_OF_URI; ++i) {
    log_result(handler.perform(csi::create_http_request(csi::http::GET, uris[i], {}, std::chrono::milliseconds(10000)), false));
  }

  while (!handler.done())
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  handler.close();
  keepalive_work.reset();
  asio_thread.join();
  return 0;
}
