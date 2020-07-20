TEST_CASE("logging::sinks")
{
    using namespace eggcpt::logging;

    set_default_sink(multi_sink{ console_sink{}, colored_console_sink{} });
}

TEST_CASE("logging::functions")
{
    EGGCPT_LOG_DEBUG("test");
    EGGCPT_LOG_INFO ("test");
    EGGCPT_LOG_WARN ("test");
    EGGCPT_LOG_ERROR("test");
    EGGCPT_LOG_FATAL("test");
}
