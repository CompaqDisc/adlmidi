#ifdef __MINGW32__
typedef struct vswprintf {} swprintf;
#endif
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <map>
#include <set>

std::map<unsigned,
    std::map<unsigned, unsigned>
        > Correlate;
unsigned maxvalues[30] = { 0 };

static const char *const MidiInsName[] = {
"AcouGrandPiano",
"BrightAcouGrand",
"ElecGrandPiano",
"Honky-tonkPiano",
"Rhodes Piano",
"Chorused Piano",
"Harpsichord",
"Clavinet",
"Celesta",
"Glockenspiel",
"Music box",
"Vibraphone",
"Marimba",
"Xylophone",
"Tubular Bells",
"Dulcimer",
"Hammond Organ",
"Percussive Organ",
"Rock Organ",
"Church Organ",
"Reed Organ",
"Accordion",
"Harmonica",
"Tango Accordion",
"Acoustic Guitar1",
"Acoustic Guitar2",
"Electric Guitar1",
"Electric Guitar2",
"Electric Guitar3",
"Overdrive Guitar",
"Distorton Guitar",
"Guitar Harmonics",
"Acoustic Bass",
"Electric Bass 1",
"Electric Bass 2",
"Fretless Bass",
"Slap Bass 1",
"Slap Bass 2",
"Synth Bass 1",
"Synth Bass 2",
"Violin",
"Viola",
"Cello",
"Contrabass",
"Tremulo Strings",
"Pizzicato String",
"Orchestral Harp",
"Timpany",
"String Ensemble1",
"String Ensemble2",
"Synth Strings 1",
"SynthStrings 2",
"Choir Aahs",
"Voice Oohs",
"Synth Voice",
"Orchestra Hit",
"Trumpet",
"Trombone",
"Tuba",
"Muted Trumpet",
"French Horn",
"Brass Section",
"Synth Brass 1",
"Synth Brass 2",
"Soprano Sax",
"Alto Sax",
"Tenor Sax",
"Baritone Sax",
"Oboe",
"English Horn",
"Bassoon",
"Clarinet",
"Piccolo",
"Flute",
"Recorder",
"Pan Flute",
"Bottle Blow",
"Shakuhachi",
"Whistle",
"Ocarina",
"Lead 1 squareea",
"Lead 2 sawtooth",
"Lead 3 calliope",
"Lead 4 chiff",
"Lead 5 charang",
"Lead 6 voice",
"Lead 7 fifths",
"Lead 8 brass",
"Pad 1 new age",
"Pad 2 warm",
"Pad 3 polysynth",
"Pad 4 choir",
"Pad 5 bowedpad",
"Pad 6 metallic",
"Pad 7 halo",
"Pad 8 sweep",
"FX 1 rain",
"FX 2 soundtrack",
"FX 3 crystal",
"FX 4 atmosphere",
"FX 5 brightness",
"FX 6 goblins",
"FX 7 echoes",
"FX 8 sci-fi",
"Sitar",
"Banjo",
"Shamisen",
"Koto",
"Kalimba",
"Bagpipe",
"Fiddle",
"Shanai",
"Tinkle Bell",
"Agogo Bells",
"Steel Drums",
"Woodblock",
"Taiko Drum",
"Melodic Tom",
"Synth Drum",
"Reverse Cymbal",
"Guitar FretNoise",
"Breath Noise",
"Seashore",
"Bird Tweet",
"Telephone",
"Helicopter",
"Applause/Noise",
"Gunshot",
// 27..34:  High Q; Slap; Scratch Push; Scratch Pull; Sticks;
//          Square Click; Metronome Click; Metronome Bell
"Ac Bass Drum",
"Bass Drum 1",
"Side Stick",
"Acoustic Snare",
"Hand Clap",
"Electric Snare",
"Low Floor Tom",
"Closed High Hat",
"High Floor Tom",
"Pedal High Hat",
"Low Tom",
"Open High Hat",
"Low-Mid Tom",
"High-Mid Tom",
"Crash Cymbal 1",
"High Tom",
"Ride Cymbal 1",
"Chinese Cymbal",
"Ride Bell",
"Tambourine",
"Splash Cymbal",
"Cow Bell",
"Crash Cymbal 2",
"Vibraslap",
"Ride Cymbal 2",
"High Bongo",
"Low Bongo",
"Mute High Conga",
"Open High Conga",
"Low Conga",
"High Timbale",
"Low Timbale",
"High Agogo",
"Low Agogo",
"Cabasa",
"Maracas",
"Short Whistle",
"Long Whistle",
"Short Guiro",
"Long Guiro",
"Claves",
"High Wood Block",
"Low Wood Block",
"Mute Cuica",
"Open Cuica",
"Mute Triangle",
"Open Triangle",
"Shaker","Jingle Bell","Bell Tree","Castanets","Mute Surdu","Open Surdu",""};

struct insdata
{
    unsigned char data[11];
    signed char   finetune;
    bool operator==(const insdata& b) const
    {
        return std::memcmp(data, b.data, 11) == 0 && finetune == b.finetune;
    }
    bool operator< (const insdata& b) const
    {
        int c = std::memcmp(data, b.data, 11);
        if(c != 0) return c < 0;
        if(finetune != b.finetune) return finetune < b.finetune;
        return 0;
    }
    bool operator!=(const insdata& b) const { return !operator==(b); }
};
struct ins
{
    size_t insno1, insno2;
    unsigned char notenum;
    bool pseudo4op;

    bool operator==(const ins& b) const
    {
        return notenum == b.notenum
        && insno1 == b.insno1
        && insno2 == b.insno2
        && pseudo4op == b.pseudo4op;
    }
    bool operator< (const ins& b) const
    {
        if(insno1 != b.insno1) return insno1 < b.insno1;
        if(insno2 != b.insno2) return insno2 < b.insno2;
        if(notenum != b.notenum) return notenum < b.notenum;
        if(pseudo4op != b.pseudo4op) return pseudo4op < b.pseudo4op;
        return 0;
    }
    bool operator!=(const ins& b) const { return !operator==(b); }
};

static std::map<insdata, std::pair<size_t, std::set<std::string> > > insdatatab;

static std::map<ins, std::pair<size_t, std::set<std::string> > > instab;
static std::map<unsigned, std::map<unsigned, size_t> > progs;

static void SetBank(unsigned bank, unsigned patch, size_t insno)
{
    progs[bank][patch] = insno+1;
}

static size_t InsertIns(
    const insdata& id,
    const insdata& id2,
    ins& in,
    const std::string& name,
    const std::string& name2 = "")
{
  if(true)
  {
    std::map<insdata, std::pair<size_t,std::set<std::string> > >::iterator
        i = insdatatab.lower_bound(id);

    size_t insno = ~0;
    if(i == insdatatab.end() || i->first != id)
    {
        std::pair<insdata, std::pair<size_t,std::set<std::string> > > res;
        res.first = id;
        res.second.first = insdatatab.size();
        if(!name.empty()) res.second.second.insert(name);
        if(!name2.empty()) res.second.second.insert(name2);
        insdatatab.insert(i, res);
        insno = res.second.first;
    }
    else
    {
        if(!name.empty()) i->second.second.insert(name);
        if(!name2.empty()) i->second.second.insert(name2);
        insno = i->second.first;
    }

    in.insno1 = insno;
  }
  if(id != id2)
  {
    std::map<insdata, std::pair<size_t,std::set<std::string> > >::iterator
        i = insdatatab.lower_bound(id2);

    size_t insno2 = ~0;
    if(i == insdatatab.end() || i->first != id2)
    {
        std::pair<insdata, std::pair<size_t,std::set<std::string> > > res;
        res.first = id2;
        res.second.first = insdatatab.size();
        if(!name.empty()) res.second.second.insert(name);
        if(!name2.empty()) res.second.second.insert(name2);
        insdatatab.insert(i, res);
        insno2 = res.second.first;
    }
    else
    {
        if(!name.empty()) i->second.second.insert(name);
        if(!name2.empty()) i->second.second.insert(name2);
        insno2 = i->second.first;
    }
    in.insno2 = insno2;
  }
  else
    in.insno2 = in.insno1;


  {
    std::map<ins, std::pair<size_t,std::set<std::string> > >::iterator
        i = instab.lower_bound(in);

    size_t resno = ~0;
    if(i == instab.end() || i->first != in)
    {
        std::pair<ins, std::pair<size_t,std::set<std::string> > > res;
        res.first = in;
        res.second.first = instab.size();
        if(!name.empty()) res.second.second.insert(name);
        if(!name2.empty()) res.second.second.insert(name2);
        instab.insert(i, res);
        resno = res.second.first;
    }
    else
    {
        if(!name.empty()) i->second.second.insert(name);
        if(!name2.empty()) i->second.second.insert(name2);
        resno = i->second.first;
    }
    return resno;
  }
}


static void LoadBNK(const char* fn, unsigned bank, const char* prefix, bool is_fat)
{
    FILE* fp = fopen(fn, "rb");
    fseek(fp, 0, SEEK_END);
    std::vector<unsigned char> data(ftell(fp));
    rewind(fp);
    fread(&data[0], 1, data.size(), fp),
    fclose(fp);

    /*printf("%s:\n", fn);*/
    unsigned short version = *(short*)&data[0]; // major,minor (2 bytes)
    //                                             "ADLIB-"    (6 bytes)
    unsigned short entries_used = *(short*)&data[8];   // entries used
    unsigned short total_entries = *(short*)&data[10]; // total entries
    unsigned       name_offset = *(unsigned*)&data[12];// name_offset
    unsigned       data_offset = *(unsigned*)&data[16];// data_offset
    // 16..23: 8 byte sof filler
    /*printf("version=%u %u %u %u %u\n",
        version, entries_used,total_entries,name_offset,data_offset);*/

    for(unsigned n=0; n<entries_used; ++n)
    {
        const size_t offset1 = name_offset + n*12;

        unsigned short data_index = data[offset1+0] + data[offset1+1]*256;
        unsigned char usage_flag = data[offset1+2];
        std::string name;
        for(unsigned p=0; p<9; ++p)
        {
            if(data[offset1+3+p] == '\0') break;
            name += char(data[offset1+3+p]);
        }

        const size_t offset2 = data_offset + data_index * 30;
        const unsigned char mode      = data[offset2+0];
        const unsigned char voice_num = data[offset2+1];
        const unsigned char* op1 = &data[offset2+2];  // 13 bytes 
        const unsigned char* op2 = &data[offset2+15];
        const unsigned char waveform_mod = data[offset2+28];
        const unsigned char waveform_car = data[offset2+29];

        /*printf("%5d %3d %8s mode=%02X voice=%02X: ", data_index,usage_flag, name.c_str(),
            mode,voice_num);*/

        bool percussive = is_fat
            ?   name[1] == 'P' /* mode == 1 */
            :   (usage_flag >= 16);

        int gmno = is_fat
            ?   ((n & 127) + percussive*128)
            :   (n + percussive*128);

        if(is_fat && percussive)
        {
            if(name[2] == 'O'
            || name[2] == 'S')
            {
                gmno = 128 + std::atoi(name.substr(3).c_str());
            }
        }

        char name2[512];
        if(is_fat)
            sprintf(name2, "%s%c%u", prefix, percussive?'P':'M', gmno&127);
        else
            sprintf(name2, "%s%u", prefix, n);

        insdata tmp;
        tmp.data[0] = (op1[ 9] << 7) // TREMOLO FLAG
                    + (op1[10] << 6) // VIBRATO FLAG
                    + (op1[ 5] << 5) // SUSTAIN FLAG
                    + (op1[11] << 4) // SCALING FLAG
                     + op1[ 1];      // FREQMUL
        tmp.data[1] = (op2[ 9] << 7)
                    + (op2[10] << 6)
                    + (op2[ 5] << 5)
                    + (op2[11] << 4)
                     + op2[ 1];
        tmp.data[2] = op1[3]*0x10 + op1[6]; // ATTACK, DECAY
        tmp.data[3] = op2[3]*0x10 + op2[6];
        tmp.data[4] = op1[4]*0x10 + op1[7]; // SUSTAIN, RELEASE
        tmp.data[5] = op2[4]*0x10 + op2[7];
        tmp.data[6] = waveform_mod;
        tmp.data[7] = waveform_car;
        tmp.data[8] = op1[0]*0x40 + op1[8]; // KSL , LEVEL
        tmp.data[9] = op2[0]*0x40 + op2[8]; // KSL , LEVEL
        tmp.data[10] = op1[2]*2 + op1[12]; // FEEDBACK, ADDITIVEFLAG
        tmp.finetune = 0;
        // Note: op2[2] and op2[12] are unused and contain garbage.
        ins tmp2;
        tmp2.notenum = is_fat ? voice_num : (percussive ? usage_flag : 0);

        if(is_fat) tmp.data[10] ^= 1;

        size_t resno = InsertIns(tmp,tmp, tmp2, std::string(1,'\377')+name, name2);

        if(!is_fat)
        {
            SetBank(bank, gmno, resno);
        }
        else
        {
            if(name[2] == 'O' || name[1] == 'M') SetBank(bank+0, gmno, resno);
            if(name[2] == 'S' || name[1] == 'M') SetBank(bank+1, gmno, resno);
        }

        /*
        for(unsigned p=0; p<30; ++p)
        {
            unsigned char value = data[offset2+p];
            if(value > maxvalues[p]) maxvalues[p] = value;

            #define dot(maxval) if(value<=maxval) printf("."); else printf("?[%u]%X",p,value);

          {
            //if(p==6 || p==7 || p==19||p==20) value=15-value;

            if(p==4 || p==10 || p==17 || p==23)// || p==25)
                printf(" %2X", value);
            else
                printf(" %X", value);
         }
        nl:;
            //if(p == 12) printf("\n%*s", 22, "");
            //if(p == 25) printf("\n%*s", 22, "");
        }
        printf("\n");
        */
    }
}

static void LoadBNK2(const char* fn, unsigned bank, const char* prefix,
                     const std::string& melo_filter,
                     const std::string& perc_filter)
{
    FILE* fp = fopen(fn, "rb");
    fseek(fp, 0, SEEK_END);
    std::vector<unsigned char> data(ftell(fp));
    rewind(fp);
    fread(&data[0], 1, data.size(), fp),
    fclose(fp);

    unsigned short ins_entries = *(unsigned short*)&data[28+2+10];
    unsigned char* records = &data[48];

    for(unsigned n=0; n<ins_entries; ++n)
    {
        const size_t offset1 = n*28;
        int used     =         records[offset1 + 15];
        int attrib   = *(int*)&records[offset1 + 16];
        int offset2  = *(int*)&records[offset1 + 20];
        if(used == 0) continue;

        std::string name;
        for(unsigned p=0; p<12; ++p)
        {
            if(records[offset1+3+p] == '\0') break;
            name += char(records[offset1+3+p]);
        }

        int gmno = 0;
        if(name.substr(0, melo_filter.size()) == melo_filter)
            gmno = std::atoi(name.substr(melo_filter.size()).c_str());
        else if(name.substr(0, perc_filter.size()) == perc_filter)
            gmno = std::atoi(name.substr(perc_filter.size()).c_str()) + 128;
        else
            continue;

        const unsigned char* insdata = &data[offset2];
        const unsigned char* ops[4] = { insdata+0, insdata+6, insdata+12, insdata+18 };
        unsigned char C4xxxFFFC = insdata[24];
        unsigned char xxP24NNN = insdata[25];
        unsigned char TTTTTTTT = insdata[26];
        unsigned char xxxxxxxx = insdata[27];

        char name2[512];
        sprintf(name2, "%s%c%u", prefix, (gmno&128)?'P':'M', gmno&127);

        struct insdata tmp[2];
        for(unsigned a=0; a<2; ++a)
        {
            tmp[a].data[0] = ops[a*2+0][0];
            tmp[a].data[1] = ops[a*2+1][0];
            tmp[a].data[2] = ops[a*2+0][2];
            tmp[a].data[3] = ops[a*2+1][2];
            tmp[a].data[4] = ops[a*2+0][3];
            tmp[a].data[5] = ops[a*2+1][3];
            tmp[a].data[6] = ops[a*2+0][4] & 0x07;
            tmp[a].data[7] = ops[a*2+1][4] & 0x07;
            tmp[a].data[8] = ops[a*2+0][1];
            tmp[a].data[9] = ops[a*2+1][1];
            tmp[a].finetune = TTTTTTTT;
        }
        tmp[0].data[10] = C4xxxFFFC & 0x0F;
        tmp[1].data[10] = (tmp[0].data[10] & 0x0E) | (C4xxxFFFC >> 7);

        ins tmp2;
        tmp2.notenum = (gmno & 128) ? 35 : 0;

        if(xxP24NNN & 8)
        {
            // dual-op
            size_t resno = InsertIns(tmp[0],tmp[1], tmp2, std::string(1,'\377')+name, name2);
            SetBank(bank, gmno, resno);
        }
        else
        {
            // single-op
            size_t resno = InsertIns(tmp[0],tmp[0], tmp2, std::string(1,'\377')+name, name2);
            SetBank(bank, gmno, resno);
        }
    }
}

struct Doom_OPL2instrument {
  unsigned char trem_vibr_1;    /* OP 1: tremolo/vibrato/sustain/KSR/multi */
  unsigned char att_dec_1;      /* OP 1: attack rate/decay rate */
  unsigned char sust_rel_1;     /* OP 1: sustain level/release rate */
  unsigned char wave_1;         /* OP 1: waveform select */
  unsigned char scale_1;        /* OP 1: key scale level */
  unsigned char level_1;        /* OP 1: output level */
  unsigned char feedback;       /* feedback/AM-FM (both operators) */
  unsigned char trem_vibr_2;    /* OP 2: tremolo/vibrato/sustain/KSR/multi */
  unsigned char att_dec_2;      /* OP 2: attack rate/decay rate */
  unsigned char sust_rel_2;     /* OP 2: sustain level/release rate */
  unsigned char wave_2;         /* OP 2: waveform select */
  unsigned char scale_2;        /* OP 2: key scale level */
  unsigned char level_2;        /* OP 2: output level */
  unsigned char unused;
  short         basenote;       /* base note offset */
} __attribute__ ((packed));

struct Doom_opl_instr {
        unsigned short        flags;
#define FL_FIXED_PITCH  0x0001          // note has fixed pitch (drum note)
#define FL_UNKNOWN      0x0002          // ??? (used in instrument #65 only)
#define FL_DOUBLE_VOICE 0x0004          // use two voices instead of one

        unsigned char         finetune;
        unsigned char         note;
        struct Doom_OPL2instrument patchdata[2];
} __attribute__ ((packed));


static void LoadDoom(const char* fn, unsigned bank, const char* prefix)
{
    FILE* fp = fopen(fn, "rb");
    fseek(fp, 0, SEEK_END);
    std::vector<unsigned char> data(ftell(fp));
    rewind(fp);
    fread(&data[0], 1, data.size(), fp),
    fclose(fp);

    for(unsigned a=0; a<175; ++a)
    {
        const size_t offset1 = 0x18A4 + a*32;
        const size_t offset2 = 8      + a*36;

        std::string name;
        for(unsigned p=0; p<32; ++p)
            if(data[offset1]!='\0')
                name += char(data[offset1+p]);

        //printf("%3d %3d %3d %8s: ", a,b,c, name.c_str());
        int gmno = a<128 ? a : ((a|128)+35);

        char name2[512]; sprintf(name2, "%s%c%u", prefix, (gmno<128?'M':'P'), gmno&127);

        Doom_opl_instr& ins = *(Doom_opl_instr*) &data[offset2];

        insdata tmp[2];
        for(unsigned index=0; index<2; ++index)
        {
            const Doom_OPL2instrument& src = ins.patchdata[index];
            tmp[index].data[0] = src.trem_vibr_1;
            tmp[index].data[1] = src.trem_vibr_2;
            tmp[index].data[2] = src.att_dec_1;
            tmp[index].data[3] = src.att_dec_2;
            tmp[index].data[4] = src.sust_rel_1;
            tmp[index].data[5] = src.sust_rel_2;
            tmp[index].data[6] = src.wave_1;
            tmp[index].data[7] = src.wave_2;
            tmp[index].data[8] = src.scale_1 | src.level_1;
            tmp[index].data[9] = src.scale_2 | src.level_2;
            tmp[index].data[10] = src.feedback;
            tmp[index].finetune = src.basenote + 12;
        }
        struct ins tmp2;
        tmp2.notenum  = ins.note;
        while(tmp2.notenum && tmp2.notenum < 20)
        {
            tmp2.notenum += 12;
            tmp[0].finetune -= 12;
            tmp[1].finetune -= 12;
        }

        if(!(ins.flags&4))
        {
            size_t resno = InsertIns(tmp[0],tmp[0],tmp2, std::string(1,'\377')+name, name2);
            SetBank(bank, gmno, resno);
        }
        else // Double instrument
        {
            tmp2.pseudo4op = true;
            size_t resno = InsertIns(tmp[0],tmp[1],tmp2, std::string(1,'\377')+name, name2);
            SetBank(bank, gmno, resno);
        }

        /*const Doom_OPL2instrument& A = ins.patchdata[0];
        const Doom_OPL2instrument& B = ins.patchdata[1];
        printf(
            "flags:%04X fine:%02X note:%02X\n"
            "{t:%02X a:%02X s:%02X w:%02X s:%02X l:%02X f:%02X\n"
            " t:%02X a:%02X s:%02X w:%02X s:%02X l:%02X ?:%02X base:%d}\n"
            "{t:%02X a:%02X s:%02X w:%02X s:%02X l:%02X f:%02X\n"
            " t:%02X a:%02X s:%02X w:%02X s:%02X l:%02X ?:%02X base:%d} ",
            ins.flags, ins.finetune, ins.note,
            A.trem_vibr_1, A.att_dec_1, A.sust_rel_1,
            A.wave_1, A.scale_1, A.level_1, A.feedback,
            A.trem_vibr_2, A.att_dec_2, A.sust_rel_2,
            A.wave_2, A.scale_2, A.level_2, A.unused, A.basenote,
            B.trem_vibr_1, B.att_dec_1, B.sust_rel_1,
            B.wave_1, B.scale_1, B.level_1, B.feedback,
            B.trem_vibr_2, B.att_dec_2, B.sust_rel_2,
            B.wave_2, B.scale_2, B.level_2, B.unused, B.basenote);
        printf(" %s VS %s\n", name.c_str(), MidiInsName[a]);
        printf("------------------------------------------------------------\n");*/
    }
}
static void LoadMiles(const char* fn, unsigned bank, const char* prefix)
{
    FILE* fp = fopen(fn, "rb");
    fseek(fp, 0, SEEK_END);
    std::vector<unsigned char> data(ftell(fp));
    rewind(fp);
    fread(&data[0], 1, data.size(), fp),
    fclose(fp);

    for(unsigned a=0; a<2000; ++a)
    {
        unsigned gmnumber  = data[a*6+0];
        unsigned gmnumber2 = data[a*6+1];
        unsigned offset    = *(unsigned*)&data[a*6+2];

        if(gmnumber == 0xFF) break;
        int gmno = gmnumber2==0x7F ? gmnumber+0x80 : gmnumber;
        int midi_index = gmno < 128 ? gmno
                       : gmno < 128+35 ? -1
                       : gmno < 128+88 ? gmno-35
                       : -1;
        unsigned length = data[offset] + data[offset+1]*256;
        signed char notenum = data[offset+2];

        /*printf("%02X %02X %08X ", gmnumber,gmnumber2, offset);
        for(unsigned b=0; b<length; ++b)
        {
            if(b > 3 && (b-3)%11 == 0) printf("\n                        ");
            printf("%02X ", data[offset+b]);
        }
        printf("\n");*/

        if(gmnumber2 != 0 && gmnumber2 != 0x7F) continue;

        char name2[512]; sprintf(name2, "%s%c%u", prefix,
            (gmno<128?'M':'P'), gmno&127);

        insdata tmp[200];

        const unsigned inscount = (length-3)/11;
        for(unsigned i=0; i<inscount; ++i)
        {
            unsigned o = offset + 3 + i*11;
            tmp[i].finetune = (gmno < 128 && i == 0) ? notenum : 0;
            tmp[i].data[0] = data[o+0];  // 20
            tmp[i].data[8] = data[o+1];  // 40 (vol)
            tmp[i].data[2] = data[o+2];  // 60
            tmp[i].data[4] = data[o+3];  // 80
            tmp[i].data[6] = data[o+4];  // E0
            tmp[i].data[1] = data[o+6];  // 23
            tmp[i].data[9] = data[o+7]; // 43 (vol)
            tmp[i].data[3] = data[o+8]; // 63
            tmp[i].data[5] = data[o+9]; // 83
            tmp[i].data[7] = data[o+10]; // E3

            unsigned fb_c = data[offset+3+5];
            tmp[i].data[10] = fb_c;
            if(i == 1)
            {
                tmp[0].data[10] = fb_c & 0x0F;
                tmp[1].data[10] = (fb_c & 0x0E) | (fb_c >> 7);
            }
        }
        if(inscount == 1) tmp[1] = tmp[0];
        if(inscount <= 2)
        {
            struct ins tmp2;
            tmp2.notenum  = gmno < 128 ? 0 : data[offset+3];
            std::string name;
            if(midi_index >= 0) name = std::string(1,'\377')+MidiInsName[midi_index];
            size_t resno = InsertIns(tmp[0], tmp[1], tmp2, name, name2);
            SetBank(bank, gmno, resno);
        }
    }
}

static void LoadIBK(const char* fn, unsigned bank, const char* prefix, bool percussive)
{
    FILE* fp = fopen(fn, "rb");
    fseek(fp, 0, SEEK_END);
    std::vector<unsigned char> data(ftell(fp));
    rewind(fp);
    fread(&data[0], 1, data.size(), fp),
    fclose(fp);

    unsigned offs1_base = 0x804, offs1_len = 9;
    unsigned offs2_base = 0x004, offs2_len = 16;

    for(unsigned a=0; a<128; ++a)
    {
        unsigned offset1 = offs1_base + a*offs1_len;
        unsigned offset2 = offs2_base + a*offs2_len;

        std::string name;
        for(unsigned p=0; p<9; ++p)
            if(data[offset1]!='\0')
                name += char(data[offset1+p]);

        int gmno = a + 128*percussive;
        int midi_index = gmno < 128 ? gmno
                       : gmno < 128+35 ? -1
                       : gmno < 128+88 ? gmno-35
                       : -1;
        char name2[512]; sprintf(name2, "%s%c%u", prefix,
            (gmno<128?'M':'P'), gmno&127);

        insdata tmp;
        tmp.data[0] = data[offset2+0];
        tmp.data[1] = data[offset2+1];
        tmp.data[8] = data[offset2+2];
        tmp.data[9] = data[offset2+3];
        tmp.data[2] = data[offset2+4];
        tmp.data[3] = data[offset2+5];
        tmp.data[4] = data[offset2+6];
        tmp.data[5] = data[offset2+7];
        tmp.data[6] = data[offset2+8];
        tmp.data[7] = data[offset2+9];
        tmp.data[10] = data[offset2+10];
        // [+11] seems to be used also, what is it for?
        tmp.finetune = 0;
        struct ins tmp2;
        tmp2.notenum  = gmno < 128 ? 0 : 35;

        size_t resno = InsertIns(tmp,tmp, tmp2, std::string(1,'\377')+name, name2);
        SetBank(bank, gmno, resno);
    }
}

static void LoadJunglevision(const char* fn, unsigned bank, const char* prefix)
{
    FILE* fp = fopen(fn, "rb");
    fseek(fp, 0, SEEK_END);
    std::vector<unsigned char> data(ftell(fp));
    rewind(fp);
    fread(&data[0], 1, data.size(), fp);
    fclose(fp);

    unsigned ins_count = data[0x20] + (data[0x21] << 8);
    unsigned drum_count = data[0x22] + (data[0x23] << 8);
    unsigned first_ins = data[0x24] + (data[0x25] << 8);
    unsigned first_drum = data[0x26] + (data[0x27] << 8);

    unsigned total_ins = ins_count + drum_count;

    for ( unsigned a = 0; a < total_ins; ++a )
    {
        unsigned offset = 0x28 + a * 0x18;
        unsigned gmno = (a < ins_count) ? (a + first_ins) : (a + first_drum);
        int midi_index = gmno < 128 ? gmno
                       : gmno < 128+35 ? -1
                       : gmno < 128+88 ? gmno-35
                       : -1;

        insdata tmp[2];

        tmp[0].data[0] = data[offset + 2];
        tmp[0].data[1] = data[offset + 8];
        tmp[0].data[2] = data[offset + 4];
        tmp[0].data[3] = data[offset + 10];
        tmp[0].data[4] = data[offset + 5];
        tmp[0].data[5] = data[offset + 11];
        tmp[0].data[6] = data[offset + 6];
        tmp[0].data[7] = data[offset + 12];
        tmp[0].data[8] = data[offset + 3];
        tmp[0].data[9] = data[offset + 9];
        tmp[0].data[10] = data[offset + 7] & ~0x30;

        tmp[1].data[0] = data[offset + 2 + 11];
        tmp[1].data[1] = data[offset + 8 + 11];
        tmp[1].data[2] = data[offset + 4 + 11];
        tmp[1].data[3] = data[offset + 10 + 11];
        tmp[1].data[4] = data[offset + 5 + 11];
        tmp[1].data[5] = data[offset + 11 + 11];
        tmp[1].data[6] = data[offset + 6 + 11];
        tmp[1].data[7] = data[offset + 12 + 11];
        tmp[1].data[8] = data[offset + 3 + 11];
        tmp[1].data[9] = data[offset + 9 + 11];
        tmp[1].data[10] = data[offset + 7 + 11] & ~0x30;

        struct ins tmp2;
        tmp2.notenum  = data[offset + 1];
        if(tmp2.notenum)
        while(tmp2.notenum < 20)
        {
            tmp2.notenum += 12;
            tmp[0].finetune -= 12;
            tmp[1].finetune -= 12;
        }

        std::string name;
        if(midi_index >= 0) name = std::string(1,'\377')+MidiInsName[midi_index];

        char name2[512]; sprintf(name2, "%s%c%u", prefix,
            (gmno<128?'M':'P'), gmno&127);

        if(!data[offset])
        {
            size_t resno = InsertIns(tmp[0],tmp[0],tmp2, name, name2);
            SetBank(bank, gmno, resno);
        }
        else // Double instrument
        {
            size_t resno = InsertIns(tmp[0],tmp[1],tmp2, name, name2);
            SetBank(bank, gmno, resno);
        }
    }
}

static void LoadTMB(const char* fn, unsigned bank, const char* prefix)
{
    FILE* fp = fopen(fn, "rb");
    fseek(fp, 0, SEEK_END);
    std::vector<unsigned char> data(ftell(fp));
    rewind(fp);
    fread(&data[0], 1, data.size(), fp);
    fclose(fp);

    for ( unsigned a = 0; a < 256; ++a )
    {
        unsigned offset = a * 0x0D;
        unsigned gmno = a;
        int midi_index = gmno < 128 ? gmno
                       : gmno < 128+35 ? -1
                       : gmno < 128+88 ? gmno-35
                       : -1;

        insdata tmp;

        tmp.data[0] = data[offset + 0];
        tmp.data[1] = data[offset + 1];
        tmp.data[2] = data[offset + 4];
        tmp.data[3] = data[offset + 5];
        tmp.data[4] = data[offset + 6];
        tmp.data[5] = data[offset + 7];
        tmp.data[6] = data[offset + 8];
        tmp.data[7] = data[offset + 9];
        tmp.data[8] = data[offset + 2];
        tmp.data[9] = data[offset + 3];
        tmp.data[10] = data[offset + 10];
        tmp.finetune = 0; //data[offset + 12];

        struct ins tmp2;
        tmp2.notenum  = data[offset + 11];

        std::string name;
        if(midi_index >= 0) name = std::string(1,'\377')+MidiInsName[midi_index];

        char name2[512]; sprintf(name2, "%s%c%u", prefix,
            (gmno<128?'M':'P'), gmno&127);

        size_t resno = InsertIns(tmp,tmp,tmp2, name, name2);
        SetBank(bank, gmno, resno);
    }
}

#include "dbopl.h"

std::vector<int> sampleBuf;
static void AddMonoAudio(unsigned long count, int* samples)
{
    sampleBuf.insert(sampleBuf.end(), samples, samples+count);
}

static void AddStereoAudio(unsigned long count, int* samples)
{
    for (unsigned long i = 0; i < count; i++)
        sampleBuf.insert(sampleBuf.end(), samples + i * 2, samples + i * 2 + 1);
}

struct DurationInfo
{
    long ms_sound_kon;
    long ms_sound_koff;
};
static DurationInfo MeasureDurations(const ins& in)
{
    insdata id[2];
    bool found[2] = {false,false};
    for(std::map<insdata, std::pair<size_t, std::set<std::string> > >
        ::const_iterator
        j = insdatatab.begin();
        j != insdatatab.end();
        ++j)
    {
        if(j->second.first == in.insno1) { id[0] = j->first; found[0]=true; if(found[1]) break; }
        if(j->second.first == in.insno2) { id[1] = j->first; found[1]=true; if(found[0]) break; }
    }
    const unsigned rate = 22050;
    const unsigned interval             = 150;
    const unsigned samples_per_interval = rate / interval;
    const int notenum =
        in.notenum < 20   ? (44 + in.notenum)
      : in.notenum >= 128 ? (44 + 128 - in.notenum)
      : in.notenum;

    DBOPL::Handler opl;
    static const short initdata[(2+3+2+2)*2] =
    { 0x004,96, 0x004,128,        // Pulse timer
      0x105, 0, 0x105,1, 0x105,0, // Pulse OPL3 enable, leave disabled
      0x001,32, 0x0BD,0           // Enable wave & melodic
    };
    opl.Init(rate);
    for(unsigned a=0; a<18; a+=2) opl.WriteReg(initdata[a], initdata[a+1]);

    const unsigned n_notes = in.insno1 == in.insno2 ? 1 : 2;
    unsigned x[2];

    if (n_notes == 2 && !in.pseudo4op)
    {
        opl.WriteReg(0x105, 1);
        opl.WriteReg(0x104, 1);
    }

    for(unsigned n=0; n<n_notes; ++n)
    {
        static const unsigned char patchdata[11] =
            {0x20,0x23,0x60,0x63,0x80,0x83,0xE0,0xE3,0x40,0x43,0xC0};
        for(unsigned a=0; a<10; ++a) opl.WriteReg(patchdata[a]+n*8, id[n].data[a]);
        opl.WriteReg(patchdata[10]+n*8, id[n].data[10] | 0x30);
        double hertz = 172.00093 * std::exp(0.057762265 * (notenum + id[n].finetune));
        if(hertz > 131071)
        {
            fprintf(stderr, "Why does note %d + finetune %d produce hertz %g?\n",
                notenum, id[n].finetune, hertz);
            hertz = 131071;
        }
        x[n] = 0x2000;
        while(hertz >= 1023.5) { hertz /= 2.0; x[n] += 0x400; } // Calculate octave
        x[n] += (int)(hertz + 0.5);

        // Keyon the note
        opl.WriteReg(0xA0+n*3, x[n]&0xFF);
        opl.WriteReg(0xB0+n*3, x[n]>>8);
    }

    const unsigned max_on = 40;
    const unsigned max_off = 60;

    // For up to 40 seconds, measure mean amplitude.
    std::vector<double> amplitudecurve_on;
    double highest_sofar = 0;
    for(unsigned period=0; period<max_on*interval; ++period)
    {
        sampleBuf.clear();
        unsigned n = samples_per_interval;
        while(n > 512)
            {opl.Generate(AddMonoAudio, AddStereoAudio, 512); n-=512;}
        if(n)opl.Generate(AddMonoAudio, AddStereoAudio, n);
        unsigned long count = sampleBuf.size();

        double mean = 0.0;
        for(unsigned long c=0; c<count; ++c)
            mean += sampleBuf[c];
        mean /= count;
        double std_deviation = 0;
        for(unsigned long c=0; c<count; ++c)
        {
            double diff = (sampleBuf[c]-mean);
            std_deviation += diff*diff;
        }
        std_deviation = std::sqrt(std_deviation / count);
        amplitudecurve_on.push_back(std_deviation);
        if(std_deviation > highest_sofar)
            highest_sofar = std_deviation;

        if(period > 6*interval && std_deviation < highest_sofar*0.2)
            break;
    }

    // Keyoff the note
    for(unsigned n=0; n<n_notes; ++n)
        opl.WriteReg(0xB0+n, (x[n]>>8) & 0xDF);

    // Now, for up to 60 seconds, measure mean amplitude.
    std::vector<double> amplitudecurve_off;
    for(unsigned period=0; period<max_off*interval; ++period)
    {
        sampleBuf.clear();
        unsigned n = samples_per_interval;
        while(n > 512)
            {opl.Generate(AddMonoAudio, AddStereoAudio, 512); n-=512;}
        if(n)opl.Generate(AddMonoAudio, AddStereoAudio, n);
        unsigned long count = sampleBuf.size();

        double mean = 0.0;
        for(unsigned long c=0; c<count; ++c)
            mean += sampleBuf[c];
        mean /= count;
        double std_deviation = 0;
        for(unsigned long c=0; c<count; ++c)
        {
            double diff = (sampleBuf[c]-mean);
            std_deviation += diff*diff;
        }
        std_deviation = std::sqrt(std_deviation / count);
        amplitudecurve_off.push_back(std_deviation);

        if(std_deviation < highest_sofar*0.2) break;
    }

    /* Analyze the results */
    double begin_amplitude        = amplitudecurve_on[0];
    double peak_amplitude_value   = begin_amplitude;
    size_t peak_amplitude_time    = 0;
    size_t quarter_amplitude_time = amplitudecurve_on.size();
    size_t keyoff_out_time        = 0;

    for(size_t a=1; a<amplitudecurve_on.size(); ++a)
    {
        if(amplitudecurve_on[a] > peak_amplitude_value)
        {
            peak_amplitude_value = amplitudecurve_on[a];
            peak_amplitude_time  = a;
        }
    }
    for(size_t a=peak_amplitude_time; a<amplitudecurve_on.size(); ++a)
    {
        if(amplitudecurve_on[a] <= peak_amplitude_value * 0.2)
        {
            quarter_amplitude_time = a;
            break;
        }
    }
    for(size_t a=0; a<amplitudecurve_off.size(); ++a)
    {
        if(amplitudecurve_off[a] <= peak_amplitude_value * 0.2)
        {
            keyoff_out_time = a;
            break;
        }
    }

    if(keyoff_out_time == 0 && amplitudecurve_on.back() < peak_amplitude_value*0.2)
        keyoff_out_time = quarter_amplitude_time;

    if(peak_amplitude_time == 0)
    {
        printf(
            "    // Amplitude begins at %6.1f,\n"
            "    // fades to 20%% at %.1fs, keyoff fades to 20%% in %.1fs.\n",
            begin_amplitude,
            quarter_amplitude_time / double(interval),
            keyoff_out_time / double(interval));
    }
    else
    {
        printf(
            "    // Amplitude begins at %6.1f, peaks %6.1f at %.1fs,\n"
            "    // fades to 20%% at %.1fs, keyoff fades to 20%% in %.1fs.\n",
            begin_amplitude,
            peak_amplitude_value,
            peak_amplitude_time / double(interval),
            quarter_amplitude_time / double(interval),
            keyoff_out_time / double(interval));
    }

    DurationInfo result;
    result.ms_sound_kon  = quarter_amplitude_time * 1000.0 / interval;
    result.ms_sound_koff = keyoff_out_time        * 1000.0 / interval;
    return result;
}

int main()
{
    printf("\
#include \"adldata.hh\"\n\
\n\
/* THIS ADLIB FM INSTRUMENT DATA IS AUTOMATICALLY GENERATED\n\
 * FROM A NUMBER OF SOURCES, MOSTLY PC GAMES.\n\
 * PREPROCESSED, CONVERTED, AND POSTPROCESSED OFF-SCREEN.\n\
 */\n\
");
    LoadMiles("opl_files/sc3.opl",  0, "G"); // Our "standard" bank!
    LoadBNK("bnk_files/melodic.bnk", 1, "HMIGM", false);
    LoadBNK("bnk_files/drum.bnk",    1, "HMIGP", false);
    LoadBNK("bnk_files/intmelo.bnk", 2, "intM", false);
    LoadBNK("bnk_files/intdrum.bnk", 2, "intP", false);
    LoadBNK("bnk_files/hammelo.bnk", 3, "hamM", false);
    LoadBNK("bnk_files/hamdrum.bnk", 3, "hamP", false);
    LoadBNK("bnk_files/rickmelo.bnk",4, "rickM", false);
    LoadBNK("bnk_files/rickdrum.bnk",4, "rickP", false);
    LoadDoom("doom2/genmidi.op2", 5, "dM");
    LoadDoom("doom2/genmidi.htc", 6, "hxM");
    LoadMiles("opl_files/warcraft.ad", 7, "sG");
    LoadMiles("opl_files/simfarm.opl", 8, "qG");
    LoadMiles("opl_files/simfarm.ad", 9, "mG");
    LoadMiles("opl_files/sample.ad", 10, "MG");
    LoadMiles("opl_files/sample.opl", 11, "oG");
    LoadMiles("opl_files/file12.opl", 12, "f12G");
    LoadMiles("opl_files/file13.opl", 13, "f13G");
    LoadMiles("opl_files/file15.opl", 14, "f15G");
    LoadMiles("opl_files/file16.opl", 15, "f16G");
    LoadMiles("opl_files/file17.opl", 16, "f17G");
    LoadMiles("opl_files/file19.opl", 17, "f19G");
    LoadMiles("opl_files/file20.opl", 18, "f20G");
    LoadMiles("opl_files/file21.opl", 19, "f21G");
    LoadMiles("opl_files/file23.opl", 20, "f23G");
    LoadMiles("opl_files/file24.opl", 21, "f24G");
    LoadMiles("opl_files/file25.opl", 22, "f25G");
    LoadMiles("opl_files/file26.opl", 23, "f26G");
    LoadMiles("opl_files/file27.opl", 24, "f27G");
    LoadMiles("opl_files/file29.opl", 25, "f29G");
    LoadMiles("opl_files/file30.opl", 26, "f30G");
    LoadMiles("opl_files/file31.opl", 27, "f31G");
    LoadMiles("opl_files/file32.opl", 28, "f32G");
    LoadMiles("opl_files/file34.opl", 29, "f34G");
    LoadMiles("opl_files/file35.opl", 30, "f35G");
    LoadMiles("opl_files/file36.opl", 31, "f36G");
    LoadMiles("opl_files/file37.opl", 32, "f37G");
    LoadMiles("opl_files/file41.opl", 33, "f41G");
    LoadMiles("opl_files/file42.opl", 34, "f42G");
    LoadMiles("opl_files/file47.opl", 35, "f47G");
    LoadMiles("opl_files/file48.opl", 36, "f48G");
    LoadMiles("opl_files/file49.opl", 37, "f49G");
    LoadMiles("opl_files/file50.opl", 38, "f50G");
    LoadMiles("opl_files/file53.opl", 39, "f53G");
    LoadMiles("opl_files/file54.opl", 40, "f54G");
    LoadBNK("bnk_files/file131.bnk", 41, "b41M", false);
    LoadBNK("bnk_files/file132.bnk", 41, "b41P", false);
    LoadBNK("bnk_files/file133.bnk", 42, "b42P", false);
    LoadBNK("bnk_files/file134.bnk", 42, "b42M", false);
    LoadBNK("bnk_files/file142.bnk", 43, "b43P", false);
    LoadBNK("bnk_files/file143.bnk", 43, "b43M", false);
    LoadBNK("bnk_files/file144.bnk", 44, "b44M", false);
    LoadBNK("bnk_files/file145.bnk", 44, "b44P", false);
    LoadBNK("bnk_files/file167.bnk", 45, "b45P", false);
    LoadBNK("bnk_files/file168.bnk", 45, "b45M", false);

    LoadBNK2("bnk_files/file159.bnk", 46, "b46", "gm","gps");
    LoadBNK2("bnk_files/file159.bnk", 47, "b47", "gm","gpo");

    LoadIBK("ibk_files/soccer-genmidi.ibk", 48, "b48", false);
    LoadIBK("ibk_files/soccer-percs.ibk",   48, "b48", true);
    LoadIBK("ibk_files/game.ibk",           49, "b49", false);
    LoadIBK("ibk_files/mt_fm.ibk",          50, "b50", false);

    LoadJunglevision("op3_files/fat2.op3", 52, "b52");
    LoadJunglevision("op3_files/fat4.op3", 53, "b53");
    LoadJunglevision("op3_files/jv_2op.op3", 54, "b54");
    LoadJunglevision("op3_files/wallace.op3", 55, "b55");

    LoadTMB("tmb_files/d3dtimbr.tmb", 56, "b56");
    LoadTMB("tmb_files/swtimbr.tmb", 57, "b57");

    LoadBNK("bnk_files/d2melo.bnk", 58, "b58M", false);
    LoadBNK("bnk_files/d2drum.bnk", 58, "b58P", false);
    LoadBNK("bnk_files/normmelo.bnk", 59, "b59M", false);
    LoadBNK("bnk_files/normdrum.bnk", 59, "b59P", false);
    LoadBNK("bnk_files/ssmelo.bnk", 60, "b60M", false);
    LoadBNK("bnk_files/ssdrum.bnk", 60, "b60P", false);

    LoadDoom("doom2/default.op2", 61, "b61");

    LoadMiles("opl_files/nemesis.opl", 62, "b62");
    LoadMiles("opl_files/nhlpa.opl", 63, "b63");

    //LoadBNK("bnk_files/grassman1.bnk", 53, "b53", false);
    //LoadBNK("bnk_files/grassman2.bnk", 52, "b52", false);

    static const char* const banknames[] =
    {"AIL (Star Control 3, Albion, Empire 2, Sensible Soccer, Settlers 2, many others)",
     "HMI (Descent, Asterix)",
     "HMI (Descent:: Int)",
     "HMI (Descent:: Ham)",
     "HMI (Descent:: Rick)",
     "DMX (Doom           :: partially pseudo 4op)",
     "DMX (Hexen, Heretic :: partially pseudo 4op)",
     "AIL (Warcraft 2)",
     "AIL (SimFarm, SimHealth :: 4op)",
     "AIL (SimFarm, Settlers, Serf City)",
     "AIL (Air Bucks, Blue And The Gray, America Invades, Terminator 2029)",
     "AIL (Ultima Underworld 2)",
     "AIL (Caesar 2 :: partially 4op, MISSING INSTRUMENTS)",   // file12
     "AIL (Death Gate)", // file13
     "AIL (Kasparov's Gambit)", // file15
     "AIL (High Seas Trader :: MISSING INSTRUMENTS)", // file16
     "AIL (Discworld, Grandest Fleet, Pocahontas, Slob Zone 3d, Ultima 4, Zorro)", // file17
     "AIL (Syndicate)", // file19
     "AIL (Guilty, Orion Conspiracy, Terra Nova Strike Force Centauri :: 4op)", // file20
     "AIL (Magic Carpet 2)", // file21
     "AIL (Jagged Alliance)", //file23
     "AIL (When Two Worlds War :: 4op, MISSING INSTRUMENTS)", //file24
     "AIL (Bards Tale Construction :: MISSING INSTRUMENTS)", //file25
     "AIL (Return to Zork)", //file26
     "AIL (Theme Hospital)", //file27
     "AIL (Inherit The Earth)", //file29
     "AIL (Inherit The Earth, file two)", //file30
     "AIL (Little Big Adventure :: 4op)", //file31
     "AIL (Wreckin Crew)", //file32
     "AIL (FIFA International Soccer)", //file34
     "AIL (Starship Invasion)", //file35
     "AIL (Super Street Fighter 2 :: 4op)", //file36
     "AIL (Lords of the Realm :: MISSING INSTRUMENTS)", //file37
     "AIL (Syndicate Wars)", //file41
     "AIL (Bubble Bobble Feat. Rainbow Islands, Z)", //file42
     "AIL (Warcraft)", //file47
     "AIL (Terra Nova Strike Force Centuri :: partially 4op)", //file48
     "AIL (System Shock :: partially 4op)", //file49
     "AIL (Advanced Civilization)", //file50
     "AIL (Battle Chess 4000 :: partially 4op, melodic only)", //file53
     "AIL (Ultimate Soccer Manager :: partially 4op)", //file54
     "HMI (Theme Park)", // file131, file132 
     "HMI (3d Table Sports, Battle Arena Toshinden)", //file133, file134
     "HMI (Aces of the Deep)", //file142, file143
     "HMI (Earthsiege)", //file144, file145
     "HMI (Anvil of Dawn)", //file167,file168
     "AIL (Master of Magic, Master of Orion 2 :: 4op, std percussion)", //file159
     "AIL (Master of Magic, Master of Orion 2 :: 4op, orchestral percussion)", //file159
     "SB (Action Soccer)",
     "SB (3d Cyberpuck :: melodic only)",
     "SB (Simon the Sorcerer :: melodic only)",
     "Bisqwit 51 (selection of 4op and 2op)",
     "OP3 (The Fat Man 2op set)",
     "OP3 (The Fat Man 4op set)",
     "OP3 (JungleVision 2op set :: melodic only)",
     "OP3 (Wallace 2op set :: melodic only)",
     "TMB (Duke Nukem 3D)",
     "TMB (Shadow Warrior)",
     "HMI (Descent 2)",
     "HMI (Normality)",
     "HMI (Shattered Steel)",
     "DMX (MUS Play :: partially pseudo 4op)",
     "AIL (Nemesis)",
     "AIL (National Hockey League PA)"
    };

#if 0
    for(unsigned a=0; a<36*8; ++a)
    {
        if( (1 << (a%8)) > maxvalues[a/8]) continue;
        
        const std::map<unsigned,unsigned>& data = Correlate[a];
        if(data.empty()) continue;
        std::vector< std::pair<unsigned,unsigned> > correlations;
        for(std::map<unsigned,unsigned>::const_iterator
            i = data.begin();
            i != data.end();
            ++i)
        {
            correlations.push_back( std::make_pair( i->second,i->first ) );
        }
        std::sort(correlations.begin(), correlations.end());
        printf("Byte %2u bit %u=mask %02X:\n", a/8, a%8, 1<<(a%8));
        for(size_t c=0; c<correlations.size() && c < 10; ++c)
        {
            unsigned count = correlations[correlations.size()-1-c ].first;
            unsigned index = correlations[correlations.size()-1-c ].second;
            printf("\tAdldata index %u, bit %u=mask %02X (%u matches)\n",
                index/8, index%8, 1 << (index%8), count);
        }
    }
#endif

    printf(
        /*
        "static const struct\n"
        "{\n"
        "    unsigned carrier_E862, modulator_E862;  // See below\n"
        "    unsigned char carrier_40, modulator_40; // KSL/attenuation settings\n"
        "    unsigned char feedconn; // Feedback/connection bits for the channel\n"
        "    signed char finetune;   // Finetune\n"
        "} adl[] =\n"*/
        "const adldata adl[%u] =\n"
        "{ //    ,---------+-------- Wave select settings\n"
        "  //    | ,-------�-+------ Sustain/release rates\n"
        "  //    | | ,-----�-�-+---- Attack/decay rates\n"
        "  //    | | | ,---�-�-�-+-- AM/VIB/EG/KSR/Multiple bits\n"
        "  //    | | | |   | | | |\n"
        "  //    | | | |   | | | |     ,----+-- KSL/attenuation settings\n"
        "  //    | | | |   | | | |     |    |    ,----- Feedback/connection bits\n"
        "  //    | | | |   | | | |     |    |    |\n", (unsigned)insdatatab.size());
    for(size_t b=insdatatab.size(), c=0; c<b; ++c)
        for(std::map<insdata,std::pair<size_t,std::set<std::string> > >
            ::const_iterator
            i = insdatatab.begin();
            i != insdatatab.end();
            ++i)
        {
            if(i->second.first != c) continue;
            printf("    { ");

            unsigned carrier_E862 =
                (i->first.data[6] << 24)
              + (i->first.data[4] << 16)
              + (i->first.data[2] << 8)
              + (i->first.data[0] << 0);
            unsigned modulator_E862 =
                (i->first.data[7] << 24)
              + (i->first.data[5] << 16)
              + (i->first.data[3] << 8)
              + (i->first.data[1] << 0);
            printf("0x%07X,0x%07X, 0x%02X,0x%02X, 0x%X,%+d",
                carrier_E862,
                modulator_E862,
                i->first.data[8],
                i->first.data[9],
                i->first.data[10],
                i->first.finetune);

            std::string names;
            for(std::set<std::string>::const_iterator
                j = i->second.second.begin();
                j != i->second.second.end();
                ++j)
            {
                if(!names.empty()) names += "; ";
                if((*j)[0] == '\377')
                    names += j->substr(1);
                else
                    names += *j;
            }
            printf(" }, // %u: %s\n", (unsigned)c, names.c_str());
        }
    printf("};\n");
    /*printf("static const struct\n"
           "{\n"
           "    unsigned short adlno1, adlno2;\n"
           "    unsigned char tone;\n"
           "    unsigned char flags;\n"
           "    long ms_sound_kon;  // Number of milliseconds it produces sound;\n"
           "    long ms_sound_koff;\n"
           "} adlins[] =\n");*/
    printf("const struct adlinsdata adlins[%u] =\n", (unsigned)instab.size());
    printf("{\n");
    for(size_t b=instab.size(), c=0; c<b; ++c)
        for(std::map<ins,std::pair<size_t,std::set<std::string> > >
            ::const_iterator
            i = instab.begin();
            i != instab.end();
            ++i)
        {
            if(i->second.first != c) continue;

            DurationInfo info = MeasureDurations(i->first);

            printf("    { ");
            printf("%3d,%3d,%3d, %1d, %6ld,%6ld",
                (unsigned) i->first.insno1,
                (unsigned) i->first.insno2,
                i->first.notenum,
                i->first.pseudo4op ? 1 : 0,
                info.ms_sound_kon,
                info.ms_sound_koff);
            std::string names;
            for(std::set<std::string>::const_iterator
                j = i->second.second.begin();
                j != i->second.second.end();
                ++j)
            {
                if(!names.empty()) names += "; ";
                if((*j)[0] == '\377')
                    names += j->substr(1);
                else
                    names += *j;
            }
            printf(" }, // %u: %s\n\n", (unsigned)c, names.c_str());
            fflush(stdout);
        }
    printf("};\n\n");

    //printf("static const unsigned short banks[][256] =\n");
    const unsigned bankcount = sizeof(banknames)/sizeof(*banknames);
    std::map<unsigned, std::vector<unsigned> > bank_data;
    for(unsigned bank=0; bank<bankcount; ++bank)
    {
        bool redundant = true;
        std::vector<unsigned> data(256);
        for(unsigned p=0; p<256; ++p)
        {
            unsigned v = progs[bank][p];
            if(v == 0)
                v = 198; // Blank.in
            else
                v -= 1;
            data[p] = v;
        }
        bank_data[bank] = data;
    }
    std::set<unsigned> listed;

    printf("const char* const banknames[%u] =\n", bankcount);
    printf("{\n");
    for(unsigned bank=0; bank<bankcount; ++bank)
        printf("    \"%s\",\n", banknames[bank]);
    printf("};\n");

    printf("const unsigned short banks[%u][256] =\n", bankcount);
    printf("{\n");
    for(unsigned bank=0; bank<bankcount; ++bank)
    {
        if(bank == 51)
        {
            // bisqwit's
    printf("\n\
    { // bank %d, Joel Yliluoma's selection\n\
0,    //GM1\n\
171,  //GM2\n\
2,    //GM3\n\
173,  //GM4\n\
653,  //GM5 rhodes DUAL\n\
654,  //GM6\n\
655,  //GM7 harps DUAL\n\
2616, //GM8 todo verify\n\
657,  //GM9 DUAL\n\
9,    //GM10\n\
180,  //GM11\n\
11,   //GM12 not sure\n\
182,  //GM13\n\
662,  //GM14\n\
663,  //GM15 DUAL  2114 is also strong\n\
185,  //GM16\n\
2115,  //GM17 DUAL\n\
2116,  //GM18 DUAL\n\
2729,  //GM19 DUAL\n\
1146,  //GM20\n\
20,    //GM21 no idea\n\
670,   //GM22 DUAL accordion\n\
671,   //GM23 DUAL\n\
672,   //GM24 DUAL no idea\n\
673,   //GM25 DUAL (ac guitar 1)\n\
674,   //GM26 DUAL\n\
675,   //GM27 DUAL\n\
676,   //GM28 DUAL\n\
677,   //GM29 DUAL (no idea)\n\
2224,  //GM30 DUAL (overdrive guitar)\n\
1497,  //GM31      (2122, 679 also strong, 2129 too)\n\
680,   //GM32 DUAL\n\
681,   //GM33 DUAL\n\
682,   //GM34 DUAL\n\
683,   //GM35 DUAL\n\
35,    //GM36\n\
2297,  //GM37 DUAL slapbass interesting\n\
34,    //GM38 no idea\n\
2130,  //GM39 synth bass\n\
688,   //GM40 DUAL synth bass 2\n\
1082,  //GM41\n\
1401,  //GM42 no idea\n\
691,   //GM43 DUAL cello\n\
692,   //GM44 DUAL slightly more contrabass-like than 42\n\
693,   //GM45 DUAL prominent sound\n\
694,   //GM46 DUAL. 2526 is strong but little too quiet on low sounds\n\
695,   //GM47 DUAL harp\n\
696,   //GM48 DUAL timpani\n\
697,   //GM49 DUAL strings1\n\
698,   //GM50 DUAL slowattack strings\n\
839,   //GM51\n\
50,    //GM52 no idea\n\
2533,  //GM53 aahs\n\
1096,  //GM54 oohs\n\
53,    //GM55\n\
704,   //GM56\n\
55,    //GM57 trumpet\n\
56,    //GM58\n\
707,   //GM59 DUAL tuba\n\
1650,  //GM60  or 1650\n\
1187,  //GM61 frhorn\n\
710,   //GM62 DUAL\n\
711,   //GM63 DUAL\n\
712,   //GM64 DUAL\n\
713,   //GM65 DUAL sopsax\n\
714,   //GM66 DUAL altosax\n\
715,   //GM67 DUAL tenosax\n\
716,   //GM68 DUAL bassax\n\
2152,  //GM69 DUAL\n\
718,   //GM70 DUAL noidea\n\
719,   //GM71 DUAL noidea\n\
720,   //GM72 DUAL clarinet\n\
71,    //GM73\n\
72,    //GM74\n\
723,   //GM75 recorder\n\
2076,  //GM76 panflute\n\
725,   //GM77 DUAL\n\
726,   //GM78 DUAL\n\
727,   //GM79 DUAL\n\
1206,  //GM80\n\
729,   //GM81 DUAL squarewave very good!\n\
730,   //GM82 DUAL sawtooth\n\
351,   //GM83\n\
82,    //GM84\n\
733,   //GM85 DUAL\n\
734,   //GM86 DUAL\n\
735,   //GM87 DUAL\n\
86,    //GM88\n\
737,   //GM89 DUAL pad new age\n\
738,   //GM90 DUAL nice adsr\n\
739,   //GM91 DUAL\n\
740,   //GM92 DUAL no idea\n\
91,    //GM93\n\
92,    //GM94\n\
868,   //GM95\n\
869,   //GM96\n\
1802,  //GM97 raindrop interesting\n\
746,   //GM98 no idea\n\
1707,  //GM99 crystal maybe like this.\n\
98,    //GM100\n\
99,    //GM101 no idea\n\
3000,  //GM102 goblins. huh??\n\
874,   //GM103\n\
875,   //GM104\n\
753,   //GM105 DUAL sitar: might work\n\
754,   //GM106 DUAL\n\
755,   //GM107 DUAL shamisen\n\
756,   //GM108 DUAL koto\n\
757,   //GM109 DUAL\n\
758,   //GM110 DUAL\n\
759,   //GM111 DUAL\n\
760,   //GM112 DUAL no idea\n\
111,   //GM113\n\
762,   //GM114 DUAL\n\
763,   //GM115 DUAL\n\
1242,  //GM116\n\
115,   //GM117 taiko, not very convincing\n\
1364,  //GM118\n\
767,   //GM119 DUAL\n\
632,   //GM120\n\
1247,  //GM121 fretnoise good\n\
120,   //GM122\n\
1807,  //GM123 seashore\n\
955,   //GM124 birdtweet\n\
1808,  //GM125 telephone\n\
1693,  //GM126 helicopter\n\
775,   //GM127\n\
776,   //GM128 gunshot\n\
377,   //GP0, bd. 2903=snare\n\
367,   //GP1\n\
2905,  //GP2\n\
1759,  //GP3\n\
377,   //GP4\n\
2908,  //GP5\n\
2909,  //GP6\n\
2087,  //GP7\n\
1760,  //GP8\n\
375,   //GP9\n\
376,   //GP10\n\
377,   //GP11\n\
377,   //GP12\n\
411,   //GP13\n\
1767,  //GP14\n\
1386,  //GP15\n\
2087,  //GP16\n\
412,   //GP17\n\
413,   //GP18\n\
412,   //GP19\n\
413,   //GP20\n\
414,   //GP21\n\
415,   //GP22\n\
416,   //GP23\n\
417,   //GP24\n\
418,   //GP25\n\
419,   //GP26\n\
281,   //GP27\n\
2473,  //GP28 slap\n\
2474,  //GP29\n\
2475,  //GP30\n\
200,   //GP31 sticks\n\
2013,  //GP32 square click\n\
2419,  //GP33 metronome click\n\
2477,  //GP34 metronome bell\n\
557,   //GP35\n\
127,   //GP36\n\
777,   //GP37 DUAL sidestick\n\
1815,  //GP38\n\
2776,  //GP39 DUAL handclap not good\n\
129,   //GP40\n\
1031,  //GP41 low-floor tom\n\
564,   //GP42 closed hihat\n\
1031,  //GP43 high-floor tom\n\
566,   //GP44 pedal hihat\n\
1031,  //GP45 low tom\n\
568,   //GP46 open hihat\n\
1031,  //GP47 low-mid tom\n\
1031,  //GP48 high-mid tom\n\
135,   //GP49 crash cymbal 1\n\
1031,  //GP50 high tom\n\
2202,  //GP51 ride cymbal 1\n\
787,   //GP52 DUAL chinese cymbal\n\
2435,  //GP53 ride bell\n\
1543,  //GP54 tamb\n\
877,   //GP55 crash cymbal\n\
1269,  //GP56 cowbell\n\
2777,  //GP57 crash cymbal 2\n\
216,   //GP58 vibraslap\n\
2209,  //GP59 ride cymbal 2\n\
580,   //GP60 hi bongo\n\
581,   //GP61 lo bongo\n\
582,   //GP62 no idea\n\
583,   //GP63\n\
584,   //GP64\n\
1275,  //GP65\n\
1275,  //GP66\n\
587,   //GP67\n\
588,   //GP68\n\
566,   //GP69\n\
2449,  //GP70\n\
229,   //GP71\n\
230,   //GP72\n\
2773,  //GP73 short guiro\n\
2774,  //GP74 long guiro\n\
329,   //GP75\n\
1284,  //GP76\n\
1284,  //GP77\n\
2718,  //GP78\n\
2719,  //GP79\n\
595,   //GP80\n\
596,   //GP81 open triangle\n\
2798,  //GP82 shaker\n\
816,   //GP83 DUAL jingle bell\n\
241,   //GP84 bell tree?\n\
1291,  //GP85 castanets\n\
169,   //GP86 mute surdu\n\
131,   //GP87 open surdu\n\
342,   //GP88\n\
343,   //GP89\n\
344,   //GP90\n\
345,   //GP91\n\
346,   //GP92\n\
420,   //GP93\n\
421,   //GP94\n\
383,   //GP95\n\
422,   //GP96\n\
423,   //GP97\n\
374,   //GP98\n\
424,   //GP99\n\
376,   //GP100\n\
425,   //GP101\n\
426,   //GP102\n\
427,   //GP103\n\
428,   //GP104\n\
429,   //GP105\n\
198,198,198,198,198, 198,198,198,198,198, //GP106..GP115\n\
198,198,198,198,198, 198,198,198,198,198, //GP116..GP125\n\
198,198 //GP126,GP127\n\
    },\n\
", bank);
            continue;
        }
        printf("    { // bank %u, %s\n", bank, banknames[bank]);
        bool redundant = true;
        for(unsigned p=0; p<256; ++p)
        {
            unsigned v = bank_data[bank][p];
            if(listed.find(v) == listed.end())
            {
                listed.insert(v);
                redundant = false;
            }
            printf("%3d,", v);
            if(p%16 == 15) printf("\n");
        }
        printf("    },\n");
        if(redundant)
        {
            printf("    // Bank %u defines nothing new.\n", bank);
            for(unsigned refbank=0; refbank<bank; ++refbank)
            {
                bool match = true;
                for(unsigned p=0; p<256; ++p)
                    if(bank_data[bank][p] != 198
                    && bank_data[bank][p] != bank_data[refbank][p])
                    {
                        match=false;
                        break;
                    }
                if(match)
                    printf("    // Bank %u is just a subset of bank %u!\n",
                        bank, refbank);
            }
        }
    }

    printf("};\n");
}
