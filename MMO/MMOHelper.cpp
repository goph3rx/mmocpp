#include <MMOHelper.h>
#include <unicode/calendar.h>
#include <unicode/parsepos.h>
#include <unicode/smpdtfmt.h>
#include <unicode/unistr.h>
#include <unicode/utypes.h>
#include <condition_variable>
#include <csignal>
#include <memory>
#include <mutex>
#include <stdexcept>

namespace
{
/**
 * Variable for controlling the shutdown of the program.
 */
auto cv = std::make_shared<std::condition_variable>();

/**
 * Default date format used for parsing and string'ifying dates.
 */
const char DATE_FORMAT[] = "yyyy-MM-dd'T'HH:mm:ssX";

/**
 * Convert the date to a timestamp.
 * As per icu docs, UDate holds the date and time as the number of
 * milliseconds since epoch. This function ignores millisecond/nanosecond part
 * for now.
 * @param date Source date.
 * @return Resulting timestamp.
 */
google::protobuf::Timestamp udate_to_timestamp(UDate date)
{
    google::protobuf::Timestamp result;
    result.set_seconds(static_cast<std::int64_t>(date / 1000));
    return result;
}

}

void mmo_wait_for_interrupt()
{
    std::signal(SIGINT, [](int) { cv->notify_all(); });
    std::signal(SIGTERM, [](int) { cv->notify_all(); });

    std::mutex mutex;
    std::unique_lock lock(mutex);
    cv->wait(lock);
}

std::string mmo_to_utf8(const std::string& source, std::string_view encoding)
{
    const icu::UnicodeString ustring(
        source.data(), static_cast<int>(source.size()), encoding.data());
    std::string result;
    ustring.toUTF8String(result);
    return result;
}

google::protobuf::Timestamp mmo_parse_timestamp(const std::string& source)
{
    // Create a date formatter
    UErrorCode error = U_ZERO_ERROR;
    auto formatter =
        icu::SimpleDateFormat(icu::UnicodeString(DATE_FORMAT), error);
    if (error > U_ZERO_ERROR)
    {
        throw std::runtime_error("Failed to create a date formatter: " +
                                 std::to_string(error));
    }

    // Parse the date
    icu::ParsePosition position;
    auto input = source + "Z";
    auto date = formatter.parse(
        icu::UnicodeString(input.data(), static_cast<int>(input.size())),
        position);
    if (position.getIndex() == 0)
    {
        throw std::runtime_error("Failed to parse the date");
    }
    return udate_to_timestamp(date);
}

std::string mmo_string_timestamp(const google::protobuf::Timestamp& timestamp)
{
    // Create a date formatter
    UErrorCode error = U_ZERO_ERROR;
    auto formatter =
        icu::SimpleDateFormat(icu::UnicodeString(DATE_FORMAT), error);
    if (error > U_ZERO_ERROR)
    {
        throw std::runtime_error("Failed to create a date formatter: " +
                                 std::to_string(error));
    }

    // Convert the timestamp to date
    // As per icu docs, UDate holds the date and time as the number of
    // milliseconds since epoch. Ignore millisecond/nanosecond part for now
    auto date = static_cast<UDate>(timestamp.seconds() * 1000);

    // Format the date
    icu::UnicodeString uresult;
    formatter.format(date, uresult);
    std::string result;
    uresult.toUTF8String(result);
    return result;
}

google::protobuf::Timestamp mmo_now_timestamp()
{
    UErrorCode error = U_ZERO_ERROR;
    auto calendar =
        std::unique_ptr<icu::Calendar>(icu::Calendar::createInstance(error));
    if (error > U_ZERO_ERROR)
    {
        throw std::runtime_error("Failed to create a calendar: " +
                                 std::to_string(error));
    }
    return udate_to_timestamp(calendar->getNow());
}
