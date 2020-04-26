#pragma once

#include <memory>
#include <functional>

#include <plog/Log.h>
#include <fmt/printf.h>

#include "MyPreciousContainer.h"


enum PreciousLogger
{
    PRECIOUS_FILE = 887,
    PRECIOUS_CONSOLE
};

// Template for fmt::fmtlib - custom type formatter
template <>
struct fmt::formatter<MyPreciousContainer>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const MyPreciousContainer& cont, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), cont.Format());
    }
};

namespace plog
{
    //Overload for plog - custom type logging
    Record& operator<<(Record& record, const MyPreciousContainer& cont)
    {
        return record << fmt::format(FMT_STRING("{}\n"), cont);
    }

    //Cusom formatter - prepares text around the log message
    class MyPreciousFormatter
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            util::nostringstream ss;            
            tm t;
            (util::localtime_s)(&t, &record.getTime().time);

            ss << fmt::format(FMT_STRING(L"{:d}-{:02d}-{:02d}, "), t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
            ss << fmt::format(FMT_STRING(L"{:02d}:{:02d}:{:02d}.{:d} "), t.tm_hour, t.tm_min, t.tm_sec, record.getTime().millitm);

            ss << severityToString(record.getSeverity()) << " ";
            ss << record.getMessage();

            return ss.str();
        }
    };

    //Custom appender - logs to the file
    template<class Formatter>
    class MyPreciousFileAppender : public IAppender
    {
    public:
        MyPreciousFileAppender() : m_file(fopen("precious_log.txt", "w+"), [](FILE* f){fclose(f);})
        {   
        }
        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format( record );
            
            fmt::fprintf( m_file.get(), str.c_str() );
        }
    private:
        std::unique_ptr<FILE, std::function<void(FILE*)>> m_file;
    };

    //Custom appender - logs to console
    template<class Formatter>
    class MyPreciousConsoleAppender : public IAppender
    {
    public:
        MyPreciousConsoleAppender() = default;

        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format(record);
            fmt::print(str.c_str());
        }
    };
}