#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <async_mqtt5.hpp>

namespace asio = boost::asio;

namespace async_mqtt5 {

template <typename StreamBase>
struct tls_handshake_type<asio::ssl::stream<StreamBase>> {
	static constexpr auto client = asio::ssl::stream_base::client;
	static constexpr auto server = asio::ssl::stream_base::server;
};

template <typename StreamBase>
void assign_tls_sni(
	const authority_path& ap,
	asio::ssl::context& /* ctx */,
	asio::ssl::stream<StreamBase>& stream
) {
	SSL_set_tlsext_host_name(stream.native_handle(), ap.host.c_str());
}

} // end namespace async_mqtt5

constexpr char mireo_ca[] =
	"-----BEGIN CERTIFICATE-----\n"
	"MIIDUTCCAjmgAwIBAgIUAzV59EhZA5MXluHNqRi9cBP0x9swDQYJKoZIhvcNAQEL\n"
	"BQAwGDEWMBQGA1UEAwwNTWlyZW8gUm9vdCBDQTAeFw0yMjA0MDcxMzM1MjlaFw0z\n"
	"MjA0MDQxMzM1MjlaMBgxFjAUBgNVBAMMDU1pcmVvIFJvb3QgQ0EwggEiMA0GCSqG\n"
	"SIb3DQEBAQUAA4IBDwAwggEKAoIBAQCin/qsHpdxT3iW0SEHhAcTfESyQcfwGtJE\n"
	"jcRrGEj36X6eahyY4AF+4Mlz2vWFeW52ayGXpQKn/z4tChdN80txdY77YmEX7XE0\n"
	"HHZYY6toNq/+mNX9h2HvB0GW+8+E0YfNN/HloTxDo3RT8+IovY9OSXt44vY4YtQK\n"
	"JbvZIm2Q8Iuv3vfNR05uFa4HcNqFhELh10jss0xG/54Y2NvB6xdKOZ8LRQuIX+Fu\n"
	"QRzMiqRFQPUJzWxbKF5I/MFiKWmAG0QNPDnlb8XtPmFTFCWY9X96wOpQOczrxT2+\n"
	"+vnTxPA3aTAkz7M4yUuocZQqTlbdfdGOSAENXavewdMCyy5bQsSLAgMBAAGjgZIw\n"
	"gY8wHQYDVR0OBBYEFLdUGYfJRf9mbM/fTav9U2vFI+TRMFMGA1UdIwRMMEqAFLdU\n"
	"GYfJRf9mbM/fTav9U2vFI+TRoRykGjAYMRYwFAYDVQQDDA1NaXJlbyBSb290IENB\n"
	"ghQDNXn0SFkDkxeW4c2pGL1wE/TH2zAMBgNVHRMEBTADAQH/MAsGA1UdDwQEAwIB\n"
	"BjANBgkqhkiG9w0BAQsFAAOCAQEAHm5d4YUP8BYcks10UCdswLtxbMUN99fNbnYo\n"
	"RMxx4EapwhEZFSNbIZvf1INJd5Po+hH5jteBeFVP+4zKqrhg3I8pjdC4josHmrhS\n"
	"28OjOFWp6xNJC43BHnLpc84bH0+XIEBbk7YA6H3GjpsZ7aJkhj/JPjjNq7bmyYN7\n"
	"1I9RK4PtIrNtUFbSsHZCZhf8Amtl8PrpktITECjfqCq+8uOAqP4McTIQ1JKwYy6f\n"
	"O6iu0eybJCFhWYENTUQyPi1VtEwOpWNLzaXBYdj69Xg8wA/J9RZIoqXWvtHv4rPF\n"
	"HGudMEIVB3y2vVLmujvQCqYPZWwbgpy5mN3F4uBNuZhTIwWRFg==\n"
	"-----END CERTIFICATE-----\n"
;


void subscribe_and_receive_openssl_tls(int num_receive) {
	std::cout << "[Test-subscribe-and-receive-openssl-tls]" << std::endl;
	using namespace async_mqtt5;
	asio::io_context ioc;

	using stream_type = asio::ssl::stream<asio::ip::tcp::socket>;
	asio::ssl::context tls_context(asio::ssl::context::tls_client);

	error_code ec;
	tls_context.add_certificate_authority(asio::buffer(mireo_ca), ec);
	tls_context.set_verify_mode(asio::ssl::verify_peer);

	using client_type = mqtt_client<stream_type, decltype(tls_context)>;
	client_type c(ioc, "", std::move(tls_context));

	c.credentials("test-subscriber-openssl-tls", "", "")
		.brokers("emqtt.mireo.local", 8883)
		.will({ "test/mqtt-test", "Client disconnected!", qos_e::at_least_once })
		.async_run(asio::detached);


	std::vector<subscribe_topic> topics;
	topics.push_back(subscribe_topic {
		"test/mqtt-test", {
			qos_e::exactly_once,
			no_local_e::no,
			retain_as_published_e::retain,
			retain_handling_e::send
		}
	});

	c.async_subscribe(
		topics, subscribe_props {},
		[](error_code ec, std::vector<reason_code> codes, suback_props) {
			if (ec == asio::error::operation_aborted)
				return;
			std::cout << "subscribe error_code: " << ec.message() << std::endl;
			std::cout << "subscribe reason_code: " << codes[0].message() << std::endl;
		}
	);


	for (auto i = 0; i < num_receive; i++) {
		c.async_receive(
			[&c, i, num_receive] (
				error_code ec, std::string topic,
				std::string payload, publish_props
			) {
				if (ec == asio::error::operation_aborted)
					return;
				std::cout << "message " << i + 1 << "/" << num_receive << std::endl;
				std::cout << "error_code: " << ec.message() << std::endl;
				std::cout << "topic: " << topic << std::endl;
				std::cout << "payload: " << payload << std::endl;

				if (i == num_receive - 1)
					c.async_disconnect(asio::detached);
			}
		);
	}

	ioc.run();
	return;
}