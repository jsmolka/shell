TEST_CASE("logging::levels")
{
    SHELL_LOG_DEBUG("debug");
    SHELL_LOG_INFO ("info");
    SHELL_LOG_WARN ("warn");
    SHELL_LOG_ERROR("error");
    SHELL_LOG_FATAL("fatal");

    shell::debug("debug");
    shell::info ("info");
    shell::warn ("warn");
    shell::error("error");
    shell::fatal("fatal");
}

TEST_CASE("logging::sinks")
{
    setSink(ConsoleSink(), ColoredConsoleSink());

    SHELL_LOG_INFO("MultiSink");

    setSink(ColoredConsoleSink());
}
