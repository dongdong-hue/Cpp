

int main(int argc, char* argv[]) {
	ip::tcp::endpoint ep(ip::address::from_string("192.168.198.142"), 8001);
	boost::shared_ptr<Client> client_ptr = boost::make_shared<Client>();
	client_ptr->start(ep);
	while(!client_ptr->connect_suc_);
	std::cout << "start finish connect_suc_: " << client_ptr->connect_suc_ << "!!!!!!!!!\n";
	while (1)
	{
		sleep(100);
	}
}