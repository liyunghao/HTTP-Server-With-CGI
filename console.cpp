#include "console.hpp"


class client 
	:public enable_shared_from_this<client>
{
public:
	client(struct host _h, tcp::socket socket, int _i): host_(_h), socket_(std::move(socket)), id(_i) {}
	
	void start() {
		// create template
		//scope();
		//ReadFile();
		//ReadPrompt();
	}
private:
	void ReadPrompt () {
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(buf, MAXLEN), 
				[this, self] (const boost::system::error_code &ec, size_t length)
				{
					string recv = string(buf);
					PrintRes(recv);
					if ( recv.find("%") == -1) {
						ReadPrompt();
					} else {
						Write();
					}
				
				});
	
	}
	void ReadFile() {
		string filename = "./testcase/" + host_.fname;
		file.open(filename);
	}

	string ReadLine() {
		string cmd;
		getline(file, cmd);
		for (auto &x : cmd) {
			if (x == '\r')
				x = 0;
		}
		return cmd;
	}

	void Write() {
		auto self(shared_from_this());
		string cmd = ReadLine();
		socket_.async_write_some(boost::asio::buffer(cmd.c_str(), cmd.size()),
				[this, self, cmd](const boost::system::error_code &ec, size_t length) {
					PrintCmd(cmd);
					if ( strncmp(cmd.c_str(), "exit", 4) != 0)
						ReadPrompt();
				});
	}
	
	void PrintCmd(string s) {
		cout << s << '\n';
	}
	void PrintRes(string s) {
		cout << s;
	}
	
	tcp::socket socket_;
	struct host host_;
	char buf[MAXLEN];
	char wbuf[MAXLEN];
	ifstream file;
	int id;
};
int main () {
	//string query = string(getenv("QUERY_STRING"));
	string query = "h0=nplinux2.cs.nctu.edu.tw&p0=1213&f0=t1.txt&h1=nplinux2.cs.nctu.edu.tw&p1=5678&f1=t2.txt&h2=&p2=&f2=&h3=&p3=&f3=&h4=&p4=&f4=";
	vector<host> hosts = parse(query);
	for (auto x : hosts) {
		x.print();
	}
	boost::asio::io_context _context;
	tcp::resolver resolver_(_context);

	cout << "Content-Type: text/html\r\n\r\n";
	scope(hosts);
	/*
	for (int i = 0; i < 5; i++) {
		if (hosts[i].hname.empty())
			break;
		
		resolver_.async_resolve(hosts[i].hname, to_string(hosts[i].port),
				[hosts, i](const boost::system::error_code& ec, tcp::resolver::results_type results) 
				{
					if (!ec) {
						shared_ptr<tcp:socket> sock = make_shared<tcp::socket>(_context);
						for (auto it = results.begin(), it != results.end(); it++) {
							sock->async_connect(it->endpoint(), 
									[sock, hosts, i](const boost::system::error_code &err)
									{
										if (!err) {
											make_shared<client>(&hosts[i], std::move(sock), i)->start();
										}
									});	
						}
					}	
				});

	}	
	*/
}
