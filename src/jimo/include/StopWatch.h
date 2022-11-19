///
/// \file StopWatch.h
///
/// \namespace jimo
/// \brief The main namespace for all classes in this library.
/// \namespace timing
/// \brief The namespace for all classes related to timing, such as
/// StopWatch and Timer.
namespace jimo::timing
{
    /// \class StopWatch
    /// \brief A stopwatch class
    ///
    /// This class provides methods for timing activities and includes a lap
    /// timer.
    class StopWatch
    {
        public:
            /// \name Constructors
            /// The constructors for the StopWatch class.
            ///@{

            /// \brief The default StopWatch constructor
            StopWatch();
            /// \brief Copy constructor
            StopWatch(const StopWatch&) = delete;
            /// \brief Move constructor
            StopWatch(StopWatch&&) = delete;
            /// \brief Copy operator=
            StopWatch& operator =(const StopWatch&) = delete;
            /// \brief Move operator=
            StopWatch& operator =(StopWatch&&) = delete;
            ///@}
    };
}