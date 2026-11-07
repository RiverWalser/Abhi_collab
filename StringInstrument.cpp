#include "Demos/AudioSystem.h"
#include "StringInstrument.h"
#include "error.h"
using namespace std;

StringInstrument::StringInstrument(double frequency) {
    /* this makes sure that the frequency is positive and that the waveform array has at leats two samples */
    if (frequency <= 0 || AudioSystem::sampleRate() / frequency < 2) {
        error("Invalid frequency.");
    }

    /* calculating the waveform length, rounding down to an integer */
    _length = AudioSystem::sampleRate() / frequency;

    /* allocating the dynamic length waveform array and initilizing all the samples to zero */
    _waveform = new Sample[_length];
    for (int i = 0; i < _length; i++) {
        _waveform[i] = 0;
    }

    /* starting our cursor at the beginning of the waveform */
    _cursor = 0;
}

StringInstrument::~StringInstrument() {
    /* deleting the dynamic waveform array */
    delete[] _waveform;
}

void StringInstrument::pluck() {
    /* filling the first half of waveform with +0.05 and  second half with -0.05 to create square wave */
    for (int i = 0; i < _length; i++) {
        if (i < _length / 2) {
            _waveform[i] = 0.05;
        } else {
            _waveform[i] = -0.05;
        }
    }

    /* reset our cursor to the beginning of the waveform */
    _cursor = 0;
}

Sample StringInstrument::nextSample() {
    /* saving the current sample before modifying the waveform */
    Sample result = _waveform[_cursor];

    /* finding  the next position */
    int next = (_cursor + 1) % _length;

    /* averaging the current and next samples, then applying decay */
    _waveform[_cursor] = 0.995 *
                         (_waveform[_cursor] + _waveform[next]) / 2.0;

    /* advance the cursor!! */
    _cursor = next;

    return result;
}

/* * * * * Test Cases Below This Point * * * * */

#include "GUI/SimpleTest.h"

/* STUDENT TEST CASES (ABHI/RIVER) */
STUDENT_TEST("Milestone 2 TC1") {
    /* checking that integer division occurs and that all samples are intitialized to zero */
    AudioSystem::setSampleRate(10);

    StringInstrument instrument(3);

    EXPECT_EQUAL(instrument._length, 3);
    EXPECT_EQUAL(instrument._cursor, 0);

    for (int i = 0; i < instrument._length; i++) {
        EXPECT_EQUAL(instrument._waveform[i], 0);
    }
}

STUDENT_TEST("Milestone 2 TC2") {
    /* checking that a waveform of length two is accepted unlike length zero/one) */
    AudioSystem::setSampleRate(10);

    StringInstrument instrument(5);

    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);
    EXPECT_EQUAL(instrument._length, 2);
    EXPECT_EQUAL(instrument._waveform[0], 0);
    EXPECT_EQUAL(instrument._waveform[1], 0);
    EXPECT_EQUAL(instrument._cursor, 0);
}

STUDENT_TEST("Milestone 3 TC1") {
    /* checking that plucking correctly fills an array with an odd number of samples!! */
    AudioSystem::setSampleRate(5);

    StringInstrument instrument(1);
    instrument.pluck();

    EXPECT_EQUAL(instrument._waveform[0], +0.05);
    EXPECT_EQUAL(instrument._waveform[1], +0.05);
    EXPECT_EQUAL(instrument._waveform[2], -0.05);
    EXPECT_EQUAL(instrument._waveform[3], -0.05);
    EXPECT_EQUAL(instrument._waveform[4], -0.05);
}

STUDENT_TEST("Milestone 3 TC2") {
    /* checking that plucking a second time restores the square wave and resets the cursor to 0 */
    AudioSystem::setSampleRate(4);

    StringInstrument instrument(1);
    instrument.pluck();
    instrument.nextSample();
    instrument.nextSample();

    instrument.pluck();

    EXPECT_EQUAL(instrument._cursor, 0);
    EXPECT_EQUAL(instrument._waveform[0], +0.05);
    EXPECT_EQUAL(instrument._waveform[1], +0.05);
    EXPECT_EQUAL(instrument._waveform[2], -0.05);
    EXPECT_EQUAL(instrument._waveform[3], -0.05);
}

STUDENT_TEST("Milestone 4 TC1") {
    /* checking that nextSample returns the original sample and also updating the waveform with the decayed value */
    AudioSystem::setSampleRate(4);

    StringInstrument instrument(1);
    instrument.pluck();

    EXPECT_EQUAL(instrument.nextSample(), +0.05);
    EXPECT_EQUAL(instrument._waveform[0], 0.05 * 0.995);

    EXPECT_EQUAL(instrument.nextSample(), +0.05);
    EXPECT_EQUAL(instrument._waveform[1], 0.0);

    EXPECT_EQUAL(instrument._cursor, 2);
}

STUDENT_TEST("Milestone 4 TC2") {
    /* checking that the cursor wraps back to zero and aslo that later samples use the updated/decayed waveform */
    AudioSystem::setSampleRate(4);

    StringInstrument instrument(1);
    instrument.pluck();

    EXPECT_EQUAL(instrument.nextSample(), +0.05);
    EXPECT_EQUAL(instrument.nextSample(), +0.05);
    EXPECT_EQUAL(instrument.nextSample(), -0.05);
    EXPECT_EQUAL(instrument.nextSample(), -0.05);

    EXPECT_EQUAL(instrument._cursor, 0);

    /* the first sample should have decayed now */
    EXPECT_EQUAL(instrument.nextSample(), 0.05 * 0.995);
    EXPECT_EQUAL(instrument._cursor, 1);
}


PROVIDED_TEST("Milestone 2: Waveform array initialized correctly.") {
    /* Change the sample rate to 3, just to make the numbers come out nice. */
    AudioSystem::setSampleRate(3);

    /* Create a string that vibrates at 1hz. This is well below the human hearing
     * threshold and exists purely for testing purposes.
     */
    StringInstrument instrument(1);

    /* Make sure something was allocated. */
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);

    /* Length should be 3 / 1 = 3. */
    EXPECT_EQUAL(instrument._length, 3);

    /* All entries should be zero. */
    EXPECT_EQUAL(instrument._waveform[0], 0);
    EXPECT_EQUAL(instrument._waveform[1], 0);
    EXPECT_EQUAL(instrument._waveform[2], 0);
}

PROVIDED_TEST("Milestone 2: Constructor reports errors on bad inputs.") {
    /* To make the math easier. */
    AudioSystem::setSampleRate(10);

    EXPECT_ERROR(StringInstrument error(-1));  // Negative frequency
    EXPECT_ERROR(StringInstrument error(0));   // Zero frequency
    EXPECT_ERROR(StringInstrument error(10));  // Array would have length 1
    EXPECT_ERROR(StringInstrument error(100)); // Array would have length 0

    /* But we shouldn't get errors for good values. */
    StringInstrument peachyKeen(1);
    EXPECT_NOT_EQUAL(peachyKeen._waveform, nullptr);
}

PROVIDED_TEST("Milestone 2: Constructor sets cursor to position 0.") {
    AudioSystem::setSampleRate(10);

    StringInstrument instrument(1);
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);

    EXPECT_EQUAL(instrument._cursor, 0);
}

PROVIDED_TEST("Milestone 3: pluck does not allocate a new array.") {
    AudioSystem::setSampleRate(10);

    StringInstrument instrument(1);
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);

    /* Plucking the string should change the contents of the array, but not
     * which array we're pointing at.
     */
    Sample* oldArray = instrument._waveform;
    instrument.pluck();

    EXPECT_EQUAL(instrument._waveform, oldArray);
}

PROVIDED_TEST("Milestone 3: pluck sets values to +0.05 and -0.05.") {
    AudioSystem::setSampleRate(4);

    /* 4 samples per sec / 1Hz = 4 samples. */
    StringInstrument instrument(1);
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);
    EXPECT_EQUAL(instrument._length, 4);

    instrument.pluck();
    EXPECT_EQUAL(instrument._waveform[0], +0.05);
    EXPECT_EQUAL(instrument._waveform[1], +0.05);
    EXPECT_EQUAL(instrument._waveform[2], -0.05);
    EXPECT_EQUAL(instrument._waveform[3], -0.05);
}

PROVIDED_TEST("Milestone 3: pluck resets the cursor.") {
    AudioSystem::setSampleRate(4);

    /* 4 samples per sec / 1Hz = 4 samples. */
    StringInstrument instrument(1);
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);

    /* Invasively move the cursor forward. This is called an "invasive"
     * test because it manipulates internal state of the type we're
     * testing, rather than just using the interface.
     */
    instrument._cursor = 3;
    instrument.pluck();
    EXPECT_EQUAL(instrument._cursor, 0);
}

PROVIDED_TEST("Milestone 4: nextSample works if pluck not called.") {
    AudioSystem::setSampleRate(10);

    /* 10 samples per sec / 1Hz = 10 samples. */
    StringInstrument instrument(1);
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);
    EXPECT_EQUAL(instrument._length, 10);

    EXPECT_EQUAL(instrument.nextSample(), 0);
    EXPECT_EQUAL(instrument.nextSample(), 0);
    EXPECT_EQUAL(instrument.nextSample(), 0);
    EXPECT_EQUAL(instrument.nextSample(), 0);
    EXPECT_EQUAL(instrument._cursor, 4);
}

PROVIDED_TEST("Milestone 4: nextSample updates waveform array.") {
    AudioSystem::setSampleRate(4);

    /* 4 samples per sec / 1Hz = 4 samples. */
    StringInstrument instrument(1);
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);
    EXPECT_EQUAL(instrument._length, 4);

    instrument.pluck();
    EXPECT_EQUAL(instrument.nextSample(), +0.05);
    EXPECT_EQUAL(instrument.nextSample(), +0.05);
    EXPECT_EQUAL(instrument.nextSample(), -0.05);
    EXPECT_EQUAL(instrument._cursor, 3);

    /* The first array value is the average of +0.05 and +0.05, scaled by 0.995.
     * The two values are the same, so we should get back +0.05 scalled by 0.995.
     */
    EXPECT_EQUAL(instrument._waveform[0], +0.05 * 0.995);

    /* The next array value is the average of +0.05 and -0.05, scaled by 0.995.
     * This is exactly zero.
     */
    EXPECT_EQUAL(instrument._waveform[1], 0.0);

    /* The next array value is the average of -0.05 and -0.05, scaled by 0.995.
     * As with the first entry, this is -0.05 scaled by 0.995.
     */
    EXPECT_EQUAL(instrument._waveform[2], -0.05 * 0.995);
}

PROVIDED_TEST("Milestone 4: nextSample wraps around properly.") {
    AudioSystem::setSampleRate(2);

    /* 2 samples per sec / 1Hz = 2 samples. */
    StringInstrument instrument(1);
    EXPECT_NOT_EQUAL(instrument._waveform, nullptr);
    EXPECT_EQUAL(instrument._length, 2);

    /* Pluck the string, forming the array [+0.05, -0.05] */
    instrument.pluck();

    /* Read two samples, which should be +0.05 and -0.05. */
    EXPECT_EQUAL(instrument.nextSample(), +0.05);
    EXPECT_EQUAL(instrument._cursor, 1);
    EXPECT_EQUAL(instrument.nextSample(), -0.05);
    EXPECT_EQUAL(instrument._cursor, 0);

    /* The first array value is the average of +0.05 and -0.05, scaled by 0.995.
     * This is zero.
     */
    EXPECT_EQUAL(instrument._waveform[0], 0.0);

    /* The next array value is the average of -0.05 and 0, scaled by 0.995. */
    Sample decayedTerm = 0.995 * (-0.05 + 0) / 2.0;
    EXPECT_EQUAL(instrument._waveform[1], decayedTerm);

    /* Get two more samples. The waveform is [0, decayedTerm], so we should
     * get back 0, then decayedTerm.
     */
    EXPECT_EQUAL(instrument.nextSample(), 0.0);
    EXPECT_EQUAL(instrument._cursor, 1);
    EXPECT_EQUAL(instrument.nextSample(), decayedTerm);
    EXPECT_EQUAL(instrument._cursor, 0);

    /* The first array value is the average of 0.0 and decayedTerm, scaled by
     * 0.995.
     */
    Sample moreDecayed = 0.995 * (decayedTerm + 0) / 2.0;
    EXPECT_EQUAL(instrument._waveform[0], moreDecayed);

    /* The second array value is the average of decayedTerm and moreDecayed,
     * scaled by 0.995.
     */
    EXPECT_EQUAL(instrument._waveform[1], 0.995 * (decayedTerm + moreDecayed) / 2.0);
}

/* * * * * Special Functions Below This Point * * * * */

#include <algorithm>

/* Copy constructor for StringInstrument. This is called automatically by C++
 * if you need to make a copy of a StringInstrument; say, if you were to pass
 * one into a function by value. Take CS106L for more details about how this
 * works.
 */
StringInstrument::StringInstrument(const StringInstrument& rhs) {
    /* Not normally part of a copy constructor, this line is designed
     * to ensure that if you haven't implemented StringInstrument,
     * you don't get weird and wild crashes.
     */
    if (rhs._waveform == nullptr) {
        return;
    }

    _length   = rhs._length;
    _waveform = new Sample[_length];
    _cursor   = rhs._cursor;

    for (int i = 0; i < _length; i++) {
        _waveform[i] = rhs._waveform[i];
    }
}

/* Assignment operator for StringInstrument. This is called automatically by
 * C++ when you assign one StringInstrument to another. Take CS106L for more
 * details about how this works. This specific implementation uses an idiom
 * called "copy-and-swap."
 */
StringInstrument& StringInstrument::operator =(StringInstrument rhs) {
    swap(_length,   rhs._length);
    swap(_waveform, rhs._waveform);
    swap(_cursor,   rhs._cursor);
    return *this;
}
