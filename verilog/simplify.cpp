#include "simplify.h"
#include <map>
#include <assert.h>
#include <algorithm>

namespace simplify_n
{
    cube_t::cube_t(int n, int v)
        : vars(n)
        , essential(false)
    {
        for(int i = 0; i != n; i++) {
            int mask = 1 << i;
            int8_t bit = (v & mask) ? k1 : k0;
            vars[i] = bit;
        }
    }

    cube_t::cube_t(const cube_t& m1, const cube_t& m2)
        : vars(m1.size())
        , essential(false)
    {
        assert(m1.size() == m2.size());
        assert(distanceOne(m1, m2));
        for(unsigned i=0; i != size(); i++) {
            const int bi = m1.bit(i);
            if(bi == m2.bit(i)) {
                vars[i] = bi;
            } else {
                vars[i] = kX;
            }
        }
    }

    bool cube_t::contains(const cube_t& m) const
    {
        assert(size() == m.size());

        for(unsigned i=0; i != size(); i++) {
            int bi = bit(i);
            if(bi != kX) {
                assert(bi == k1 || bi == k0);
                if(m.bit(i) != bi) return false;
            }
        }
        return true;
    }

    bool cube_t::covers(int input) const
    {
        int n = 1 << size();
        assert(input < n);

        for(unsigned i=0; i != size(); i++) {
            int b = (input & (1 << i)) ? 1 : 0;
            if((bit(i) == k1) && (b == 0)) return false;
            if((bit(i) == k0) && (b == 1)) return false;
        }
        return true;
    }

    unsigned cube_t::get_uncovered_count(intset_t& covered) const
    {
        unsigned cnt=0;
        unsigned n = 1 << size();
        for(unsigned i=0; i != n; i++) {
            if(covers(i)) {
                if(covered.find(i) == covered.end()) {
                    cnt+=1;
                }
            }
        }
        return cnt;
    }

    bool cube_t::operator==(const cube_t& m) const
    {
        if(size() != m.size()) return false;
        for(unsigned i=0; i != size(); i++) {
            if(bit(i) != m.bit(i)) { return false; }
        }
        return true;
    }

    bool exists(cube_list_t& ms, cube_t& m)
    {
        for(unsigned i=0; i != ms.size(); i++) {
            if(ms[i] == m) return true;
        }
        return false;
    }

    bool distanceOne(const cube_t& m1, const cube_t& m2)
    {
        assert(m1.size() == m2.size());
        // should never happen.
        if(m1.size() != m2.size()) return false;

        int dist = 0;
        for(unsigned i=0; i != m1.size(); i++) {
            int b1 = m1.bit(i);
            int b2 = m2.bit(i);
            if(b1 != b2)
            {
                dist += 1;
            }
        }
        return dist == 1;
    }

    std::ostream& operator<<(std::ostream& out, const cube_t& m)
    {
        assert(m.size() <= 26);
        const char* letters = "abcdefghijklmnopqrstuvwxyz";

        for(unsigned i=0; i != m.size(); i++) {
            int b = m.bit(i);
            if(b == k0) {
                out << "!" << letters[i];
            } else if(b == k1) {
                out << " " << letters[i];
            } else {
                assert(b == kX);
                out << " -";
            }
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const cube_list_t& ms)
    {
        for(unsigned i=0; i != ms.size(); i++) {
            out << ms[i] << " ";
        }
        return out;
    }

    void simplify(cube_list_t& terms)
    {
        assert(terms.size() > 0);

        iterated_consensus(terms);

        typedef std::vector< std::vector<int> > array2d_t;
        array2d_t covermap;

        for(unsigned i=0; i != terms.size(); i++) {
            unsigned n = 1 << terms[i].size();
            if(covermap.size() == 0) { covermap.resize(n); } 
            else { assert(covermap.size() == n); }

            for(unsigned j=0; j != n; j++) {
                if(terms[i].covers(j)) {
                    covermap[j].push_back(i);
                }
            }
        }
        for(unsigned i=0; i != covermap.size(); i++) {
            if(covermap[i].size() == 1) {
                terms[covermap[i][0]].mark_essential();
            }
        }

        cube_list_t result;
        intset_t covered;
        unsigned n = 0;
        for(unsigned i=0; i != terms.size(); i++) {
            if(terms[i].is_essential()) {
                result.push_back(terms[i]);
                if(n == 0) {
                    n = 1 << terms[i].size();
                } else {
                    assert((int)n == (1 << terms[i].size()));
                }
                for(unsigned j=0; j != n; j++) {
                    if(terms[i].covers(j)) {
                        covered.insert(j);
                    }
                }
            }
        }

        // now repeat.
        while(true)
        {
            unsigned max_cnt = 0;
            int max_idx = -1; 

            for(unsigned i=0; i != terms.size(); i++) {
                unsigned cnt = terms[i].get_uncovered_count(covered);
                if(cnt > max_cnt) {
                    cnt = max_cnt;
                    max_idx = i;
                }
            }

            if(max_idx != -1) {
                cube_t& t = terms[max_idx];
                result.push_back(t);
                for(unsigned j=0; j != n; j++) {
                    if(t.covers(j)) {
                        covered.insert(j);
                    }
                }
            } else {
                break;
            }
        }

        terms.clear();
        for(unsigned i=0; i != result.size(); i++) {
            terms.push_back(result[i]);
        }
    }

    void iterated_consensus(cube_list_t& terms)
    {
        while(true) {
            bool new_added = false;

            cube_list_t new_terms(terms);
            for(unsigned i=0; i != terms.size(); i++) {
                cube_t& mi = terms[i];
                for(unsigned j=0; j != terms.size(); j++) {
                    cube_t& mj = terms[j];
                    if(distanceOne(mi, mj)) {
                        cube_t c(mi, mj);
                        if(!exists(new_terms, c)) {
                            new_terms.push_back(c);
                            new_added = true;
                        }
                    }
                }
            }

            terms.clear();
            for(unsigned i=0; i != new_terms.size(); i++) {
                bool contained = false;
                cube_t& mi = new_terms[i];
                for(unsigned j=0; j != new_terms.size(); j++) {
                    cube_t& mj = new_terms[j];
                    if(i != j && mj.contains(mi)) {
                        contained = true;
                    }
                }
                if(!contained) {
                    terms.push_back(mi);
                } 
            }

            if(!new_added) break;
        }
    }
}

