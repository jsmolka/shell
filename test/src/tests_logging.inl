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
    set_default_sink(MultiSink(ConsoleSink(), ColoredConsoleSink()));

    EGGCPT_LOG_INFO("MultiSink");

    set_default_sink(ColoredConsoleSink());
}
