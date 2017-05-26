#include "counter_point.hpp"

CounterPoint::CounterPoint()
{

}

void
CounterPoint::SetMaterial(MuMaterial material)
{
    this->material = material;

    long number_of_notes = material.NumberOfNotes();

    for (long index = 0; index < number_of_notes; index++)
    {
        vector<int> harmonic_range = GetHarmonicRange(index);
    }
}

vector<int>
CounterPoint::GetHarmonicRange (int note_index)
{
    int note_pitch = this->material.GetNote(note_index).Pitch();

    int unison = note_pitch;
    int third_minor = note_pitch - 3;
    int third_major = note_pitch - 4;
    int fifith_perfect = note_pitch - 7;
    int sixth_minor = note_pitch - 8;
    int sixth_major = note_pitch - 9;

    vector<int> harmonic_range;
    bool is_first_species = note_index == 0;

    if(is_first_species) {
        harmonic_range = { unison, third_minor, third_major, fifith_perfect,
                           sixth_minor, sixth_major };
    } else {
        harmonic_range = { third_minor, third_major, fifith_perfect,
                           sixth_minor, sixth_major };
    }

    cout << "note_pitch: " << note_pitch << "  harmonic_range: ";
    for (int j = 0; j < harmonic_range.size(); j++)
    {
        cout << harmonic_range[j] << " ";
    }
    cout << endl;

    return harmonic_range;
}
