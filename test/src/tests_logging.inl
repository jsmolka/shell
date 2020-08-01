namespace tests_logging
{

TEST_CASE("logging::levels")
{
    EGGCPT_LOG_DEBUG("debug");
    EGGCPT_LOG_INFO ("info");
    EGGCPT_LOG_WARN ("warn");
    EGGCPT_LOG_ERROR("error");
    EGGCPT_LOG_FATAL("fatal");
}

TEST_CASE("logging::sinks")
{
    set_default_sink(multi_sink{ console_sink{}, colored_console_sink{} });

    EGGCPT_LOG_INFO("multi_sink");

    set_default_sink(colored_console_sink{});
}

}  // namespace tests_logging
