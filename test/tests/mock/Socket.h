#pragma once

class MockSocket : public Socket {
 public:
  MOCK_METHOD2(read,
      void(std::vector<char>& message, boost_read_callback callback));
  MOCK_METHOD2(write,
      void(const std::string& message, boost_write_callback callback));
  MOCK_METHOD0(close,
      void());
};
