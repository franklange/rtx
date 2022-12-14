#include <rtx/broker.hpp>

#include <bln_net/udp_socket_MOCK.hpp>
#include <gtest/gtest.h>

#include <string>

namespace rtx::test {

struct broker_test : public ::testing::Test
{
    using socket = bln_net::udp::socket_MOCK;
    using socket_mock = ::testing::StrictMock<socket>;

    socket_mock m_socket;
    broker m_broker{m_socket};

    const topic m_topic_1{"foo.bar"};
    const topic m_topic_2{"company.staff"};

    const remote m_client_1{"10.0.0.1", 9000};
    const remote m_client_2{"10.0.0.2", 8000};

    const std::string m_msg{"thisisamessage"};
};

TEST_F(broker_test, num_topics_empty)
{
    EXPECT_EQ(0, m_broker.num_topics());
}

TEST_F(broker_test, num_clients_empty)
{
    EXPECT_EQ(0, m_broker.num_clients(m_topic_1));
}

TEST_F(broker_test, num_clients_unknown)
{
    EXPECT_EQ(0, m_broker.num_clients(m_topic_1));
}

TEST_F(broker_test, sub)
{
    m_broker.process(subscribe{m_client_1, m_topic_1});

    EXPECT_EQ(1, m_broker.num_topics());
    EXPECT_EQ(1, m_broker.num_clients(m_topic_1));
}

TEST_F(broker_test, sub_multi)
{
    m_broker.process(subscribe{m_client_1, m_topic_1});
    m_broker.process(subscribe{m_client_2, m_topic_1});
    m_broker.process(subscribe{m_client_2, m_topic_2});

    EXPECT_EQ(2, m_broker.num_topics());
    EXPECT_EQ(2, m_broker.num_clients(m_topic_1));
    EXPECT_EQ(1, m_broker.num_clients(m_topic_2));
}

TEST_F(broker_test, unsub_empty)
{
    m_broker.process(unsubscribe{m_client_1, m_topic_1});

    EXPECT_EQ(0, m_broker.num_topics());
}

TEST_F(broker_test, sub_unsub)
{
    m_broker.process(subscribe{m_client_1, m_topic_1});
    m_broker.process(unsubscribe{m_client_1, m_topic_1});

    EXPECT_EQ(0, m_broker.num_topics());
}

TEST_F(broker_test, sub_unsub_multi)
{
    m_broker.process(subscribe{m_client_1, m_topic_1});
    m_broker.process(subscribe{m_client_2, m_topic_1});
    m_broker.process(subscribe{m_client_2, m_topic_2});

    m_broker.process(unsubscribe{m_client_1, m_topic_1});

    EXPECT_EQ(2, m_broker.num_topics());
    EXPECT_EQ(1, m_broker.num_clients(m_topic_1));
    EXPECT_EQ(1, m_broker.num_clients(m_topic_2));
}

TEST_F(broker_test, publish_empty)
{
    m_broker.process(publish{m_client_1, m_topic_1, m_msg});
}

TEST_F(broker_test, publish_self)
{
    m_broker.process(subscribe{m_client_1, m_topic_1});
    m_broker.process(publish{m_client_1, m_topic_1, m_msg});
}

TEST_F(broker_test, publish)
{
    m_broker.process(subscribe{m_client_1, m_topic_1});

    EXPECT_CALL(m_socket, put_rv).Times(1);
    m_broker.process(publish{m_client_2, m_topic_1, m_msg});
}

TEST_F(broker_test, publish_multi)
{
    m_broker.process(subscribe{m_client_1, m_topic_1});
    m_broker.process(subscribe{m_client_2, m_topic_1});
    m_broker.process(subscribe{m_client_2, m_topic_2});

    EXPECT_CALL(m_socket, put_rv).Times(1);
    m_broker.process(publish{m_client_1, m_topic_1, m_msg});
}

} // namespace rtx::test
