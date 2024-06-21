#ifndef BITBOARD_H_DE3A36A4E0B0
#define BITBOARD_H_DE3A36A4E0B0

#include <string>
#include "types.h"


constexpr uint64_t RANK1_BB = 0xff;
constexpr uint64_t RANK2_BB = RANK1_BB << 8;
constexpr uint64_t RANK3_BB = RANK1_BB << 16;
constexpr uint64_t RANK4_BB = RANK1_BB << 24;
constexpr uint64_t RANK5_BB = RANK1_BB << 32;
constexpr uint64_t RANK6_BB = RANK1_BB << 40;
constexpr uint64_t RANK7_BB = RANK1_BB << 48;
constexpr uint64_t RANK8_BB = RANK1_BB << 56;

constexpr uint64_t FILEA_BB = 0x0101010101010101ull;
constexpr uint64_t FILEB_BB = FILEA_BB << 1;
constexpr uint64_t FILEC_BB = FILEA_BB << 2;
constexpr uint64_t FILED_BB = FILEA_BB << 3;
constexpr uint64_t FILEE_BB = FILEA_BB << 4;
constexpr uint64_t FILEF_BB = FILEA_BB << 5;
constexpr uint64_t FILEG_BB = FILEA_BB << 6;
constexpr uint64_t FILEH_BB = FILEA_BB << 7;


extern uint8_t POPCOUNT16[65536];
constexpr int popcount(uint64_t bb)
{
	union
	{
		uint64_t whole;
		uint16_t quarters[4];
	};
	whole = bb;
	return POPCOUNT16[quarters[0]] 
		 + POPCOUNT16[quarters[1]] 
		 + POPCOUNT16[quarters[2]] 
		 + POPCOUNT16[quarters[3]];
}


constexpr uint64_t square_to_bb(Square s) { return 1ull << s; }
constexpr Square make_square(File f, Rank r) { return Square((r << 3) + f); }

constexpr uint64_t operator&(uint64_t bb, Square s) { return bb & square_to_bb(s); }
constexpr uint64_t operator|(uint64_t bb, Square s) { return bb | square_to_bb(s); }
constexpr uint64_t operator^(uint64_t bb, Square s) { return bb ^ square_to_bb(s); }
constexpr uint64_t operator&=(uint64_t& bb, Square s) { return bb &= square_to_bb(s); }
constexpr uint64_t operator|=(uint64_t& bb, Square s) { return bb |= square_to_bb(s); }
constexpr uint64_t operator^=(uint64_t& bb, Square s) { return bb ^= square_to_bb(s); }

constexpr uint64_t operator&(Square s1, Square s2) { return square_to_bb(s1) & square_to_bb(s2); }
constexpr uint64_t operator|(Square s1, Square s2) { return square_to_bb(s1) | square_to_bb(s2); }
constexpr uint64_t operator^(Square s1, Square s2) { return square_to_bb(s1) ^ square_to_bb(s2); }

constexpr File file_of(Square s) { return File(s & 7); }
constexpr Rank rank_of(Square s) { return Rank(s >> 3); }

constexpr uint64_t file_bb(File f) { return FILEA_BB << f; }
constexpr uint64_t rank_bb(Rank r) { return RANK1_BB << (8 * r); }

constexpr uint64_t file_bb(Square s) { return file_bb(file_of(s)); }
constexpr uint64_t rank_bb(Square s) { return rank_bb(rank_of(s)); }

constexpr bool is_ok(Square s) { return s >= A1 && s <= H8; }

extern uint8_t SQUARE_DISTANCE[64][64];
extern uint8_t CENTER_DISTANCE[64];

void init_bitboards();

std::string bb_to_string(uint64_t bb);
std::string sq_to_string(Square s);


template<Direction D>
constexpr uint64_t shift(uint64_t bb)
{
	switch (D)
	{
		case N: return bb << 8;
	 	case S: return bb >> 8;
	 	case NN: return bb << 16;
	 	case SS: return bb >> 16;
	 	case E: return (bb & ~FILEH_BB) << 1;
	 	case W: return (bb & ~FILEA_BB) >> 1;
	 	case NE: return (bb & ~FILEH_BB) << 9;
	 	case NW: return (bb & ~FILEA_BB) << 7;
	 	case SE: return (bb & ~FILEH_BB) >> 7;
	 	case SW: return (bb & ~FILEA_BB) >> 9;
		default: return 0ull;
	}
}


constexpr uint64_t pawn_attacks_bb(Color c, uint64_t bb)
{
	return c == WHITE ? shift<NW>(bb) | shift<NE>(bb)
				   	  : shift<SW>(bb) | shift<SE>(bb);
}

constexpr Direction pawn_push(Color c) { return c == WHITE ? N : S; }

constexpr uint64_t PSEUDO_ATTACKS[7][64] = 
{
	{ // CRAP
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	},
	{ // PAWN
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	}, 
	{ // KNIGHT
		0x20400ull, 0x50800ull, 0xa1100ull, 0x142200ull, 0x284400ull, 0x508800ull, 0xa01000ull, 0x402000ull,
		0x2040004ull, 0x5080008ull, 0xa110011ull, 0x14220022ull, 0x28440044ull, 0x50880088ull, 0xa0100010ull, 0x40200020ull,
		0x204000402ull, 0x508000805ull, 0xa1100110aull, 0x1422002214ull, 0x2844004428ull, 0x5088008850ull, 0xa0100010a0ull, 0x4020002040ull,
		0x20400040200ull, 0x50800080500ull, 0xa1100110a00ull, 0x142200221400ull, 0x284400442800ull, 0x508800885000ull, 0xa0100010a000ull, 0x402000204000ull,
		0x2040004020000ull, 0x5080008050000ull, 0xa1100110a0000ull, 0x14220022140000ull, 0x28440044280000ull, 0x50880088500000ull, 0xa0100010a00000ull, 0x40200020400000ull,
		0x204000402000000ull, 0x508000805000000ull, 0xa1100110a000000ull, 0x1422002214000000ull, 0x2844004428000000ull, 0x5088008850000000ull, 0xa0100010a0000000ull, 0x4020002040000000ull,
		0x400040200000000ull, 0x800080500000000ull, 0x1100110a00000000ull, 0x2200221400000000ull, 0x4400442800000000ull, 0x8800885000000000ull, 0x100010a000000000ull, 0x2000204000000000ull,
		0x4020000000000ull, 0x8050000000000ull, 0x110a0000000000ull, 0x22140000000000ull, 0x44280000000000ull, 0x88500000000000ull, 0x10a00000000000ull, 0x20400000000000ull,
	},
	{ // BISHOP
		0x8040201008040200ull, 0x80402010080500ull, 0x804020110a00ull, 0x8041221400ull, 0x182442800ull, 0x10204885000ull, 0x102040810a000ull, 0x102040810204000ull,
		0x4020100804020002ull, 0x8040201008050005ull, 0x804020110a000aull, 0x804122140014ull, 0x18244280028ull, 0x1020488500050ull, 0x102040810a000a0ull, 0x204081020400040ull,
		0x2010080402000204ull, 0x4020100805000508ull, 0x804020110a000a11ull, 0x80412214001422ull, 0x1824428002844ull, 0x102048850005088ull, 0x2040810a000a010ull, 0x408102040004020ull,
		0x1008040200020408ull, 0x2010080500050810ull, 0x4020110a000a1120ull, 0x8041221400142241ull, 0x182442800284482ull, 0x204885000508804ull, 0x40810a000a01008ull, 0x810204000402010ull,
		0x804020002040810ull, 0x1008050005081020ull, 0x20110a000a112040ull, 0x4122140014224180ull, 0x8244280028448201ull, 0x488500050880402ull, 0x810a000a0100804ull, 0x1020400040201008ull,
		0x402000204081020ull, 0x805000508102040ull, 0x110a000a11204080ull, 0x2214001422418000ull, 0x4428002844820100ull, 0x8850005088040201ull, 0x10a000a010080402ull, 0x2040004020100804ull,
		0x200020408102040ull, 0x500050810204080ull, 0xa000a1120408000ull, 0x1400142241800000ull, 0x2800284482010000ull, 0x5000508804020100ull, 0xa000a01008040201ull, 0x4000402010080402ull,
		0x2040810204080ull, 0x5081020408000ull, 0xa112040800000ull, 0x14224180000000ull, 0x28448201000000ull, 0x50880402010000ull, 0xa0100804020100ull, 0x40201008040201ull,
	},
	{ // ROOK
		0x1010101010101feull, 0x2020202020202fdull, 0x4040404040404fbull, 0x8080808080808f7ull, 0x10101010101010efull, 0x20202020202020dfull, 0x40404040404040bfull, 0x808080808080807full,
		0x10101010101fe01ull, 0x20202020202fd02ull, 0x40404040404fb04ull, 0x80808080808f708ull, 0x101010101010ef10ull, 0x202020202020df20ull, 0x404040404040bf40ull, 0x8080808080807f80ull,
		0x101010101fe0101ull, 0x202020202fd0202ull, 0x404040404fb0404ull, 0x808080808f70808ull, 0x1010101010ef1010ull, 0x2020202020df2020ull, 0x4040404040bf4040ull, 0x80808080807f8080ull,
		0x1010101fe010101ull, 0x2020202fd020202ull, 0x4040404fb040404ull, 0x8080808f7080808ull, 0x10101010ef101010ull, 0x20202020df202020ull, 0x40404040bf404040ull, 0x808080807f808080ull,
		0x10101fe01010101ull, 0x20202fd02020202ull, 0x40404fb04040404ull, 0x80808f708080808ull, 0x101010ef10101010ull, 0x202020df20202020ull, 0x404040bf40404040ull, 0x8080807f80808080ull,
		0x101fe0101010101ull, 0x202fd0202020202ull, 0x404fb0404040404ull, 0x808f70808080808ull, 0x1010ef1010101010ull, 0x2020df2020202020ull, 0x4040bf4040404040ull, 0x80807f8080808080ull,
		0x1fe010101010101ull, 0x2fd020202020202ull, 0x4fb040404040404ull, 0x8f7080808080808ull, 0x10ef101010101010ull, 0x20df202020202020ull, 0x40bf404040404040ull, 0x807f808080808080ull,
		0xfe01010101010101ull, 0xfd02020202020202ull, 0xfb04040404040404ull, 0xf708080808080808ull, 0xef10101010101010ull, 0xdf20202020202020ull, 0xbf40404040404040ull, 0x7f80808080808080ull,
	},
	{ // QUEEN
		0x81412111090503feull, 0x2824222120a07fdull, 0x404844424150efbull, 0x8080888492a1cf7ull, 0x10101011925438efull, 0x2020212224a870dfull, 0x404142444850e0bfull, 0x8182848890a0c07full,
		0x412111090503fe03ull, 0x824222120a07fd07ull, 0x4844424150efb0eull, 0x80888492a1cf71cull, 0x101011925438ef38ull, 0x20212224a870df70ull, 0x4142444850e0bfe0ull, 0x82848890a0c07fc0ull,
		0x2111090503fe0305ull, 0x4222120a07fd070aull, 0x844424150efb0e15ull, 0x888492a1cf71c2aull, 0x1011925438ef3854ull, 0x212224a870df70a8ull, 0x42444850e0bfe050ull, 0x848890a0c07fc0a0ull,
		0x11090503fe030509ull, 0x22120a07fd070a12ull, 0x4424150efb0e1524ull, 0x88492a1cf71c2a49ull, 0x11925438ef385492ull, 0x2224a870df70a824ull, 0x444850e0bfe05048ull, 0x8890a0c07fc0a090ull,
		0x90503fe03050911ull, 0x120a07fd070a1222ull, 0x24150efb0e152444ull, 0x492a1cf71c2a4988ull, 0x925438ef38549211ull, 0x24a870df70a82422ull, 0x4850e0bfe0504844ull, 0x90a0c07fc0a09088ull,
		0x503fe0305091121ull, 0xa07fd070a122242ull, 0x150efb0e15244484ull, 0x2a1cf71c2a498808ull, 0x5438ef3854921110ull, 0xa870df70a8242221ull, 0x50e0bfe050484442ull, 0xa0c07fc0a0908884ull,
		0x3fe030509112141ull, 0x7fd070a12224282ull, 0xefb0e1524448404ull, 0x1cf71c2a49880808ull, 0x38ef385492111010ull, 0x70df70a824222120ull, 0xe0bfe05048444241ull, 0xc07fc0a090888482ull,
		0xfe03050911214181ull, 0xfd070a1222428202ull, 0xfb0e152444840404ull, 0xf71c2a4988080808ull, 0xef38549211101010ull, 0xdf70a82422212020ull, 0xbfe0504844424140ull, 0x7fc0a09088848281ull,
	},
	{ // KING
		0x302ull, 0x705ull, 0xe0aull, 0x1c14ull, 0x3828ull, 0x7050ull, 0xe0a0ull, 0xc040ull,
		0x30203ull, 0x70507ull, 0xe0a0eull, 0x1c141cull, 0x382838ull, 0x705070ull, 0xe0a0e0ull, 0xc040c0ull,
		0x3020300ull, 0x7050700ull, 0xe0a0e00ull, 0x1c141c00ull, 0x38283800ull, 0x70507000ull, 0xe0a0e000ull, 0xc040c000ull,
		0x302030000ull, 0x705070000ull, 0xe0a0e0000ull, 0x1c141c0000ull, 0x3828380000ull, 0x7050700000ull, 0xe0a0e00000ull, 0xc040c00000ull,
		0x30203000000ull, 0x70507000000ull, 0xe0a0e000000ull, 0x1c141c000000ull, 0x382838000000ull, 0x705070000000ull, 0xe0a0e0000000ull, 0xc040c0000000ull,
		0x3020300000000ull, 0x7050700000000ull, 0xe0a0e00000000ull, 0x1c141c00000000ull, 0x38283800000000ull, 0x70507000000000ull, 0xe0a0e000000000ull, 0xc040c000000000ull,
		0x302030000000000ull, 0x705070000000000ull, 0xe0a0e0000000000ull, 0x1c141c0000000000ull, 0x3828380000000000ull, 0x7050700000000000ull, 0xe0a0e00000000000ull, 0xc040c00000000000ull,
		0x203000000000000ull, 0x507000000000000ull, 0xa0e000000000000ull, 0x141c000000000000ull, 0x2838000000000000ull, 0x5070000000000000ull, 0xa0e0000000000000ull, 0x40c0000000000000ull,
	},
};


static constexpr uint64_t ROOK_MAGIC_NUMBERS[64] = 
{
	0x2080019880214000ull, 0x40004020001000ull, 0x81000830c1200300ull, 0x200100420400a00ull, 0x100020410080100ull, 0x1080020080010400ull, 0x8400009001241228ull, 0x200002084004902ull,
	0x800800080204001ull, 0x400050002000ull, 0x1201802000811001ull, 0x211001000200900ull, 0x800800800400ull, 0x408803200040080ull, 0x5504003214100108ull, 0x1001082005100ull,
	0x2920208000804004ull, 0x190024000442000ull, 0x1900480200180b2ull, 0x109010008100420ull, 0x4c008080040800ull, 0x80104401020ull, 0x40090420108ull, 0x304a0004124081ull,
	0x891a830200244201ull, 0x1004200220088ull, 0x2000200480100180ull, 0x9010001080800800ull, 0xa040080800800ull, 0x4002004200702844ull, 0x1001000100040200ull, 0x8000042000c0091ull,
	0xc080002000400056ull, 0x4000200080804002ull, 0x300100082802000ull, 0x10040800801080ull, 0x8010041101000800ull, 0x31000401000208ull, 0x20800200800100ull, 0x8800940082000469ull,
	0x140802040008002ull, 0x4000500020004000ull, 0x40100020008080ull, 0x601001000090020ull, 0x84004080080800ull, 0x204004080110ull, 0x1008010002008080ull, 0xa00010040a20004ull,
	0x48fffe99fecfaa00ull, 0x48fffe99fecfaa00ull, 0x497fffadff9c2e00ull, 0x613fffddffce9200ull, 0xffffffe9ffe7ce00ull, 0xfffffff5fff3e600ull, 0x3ff95e5e6a4c0ull, 0x510ffff5f63c96a0,
	0xebffffb9ff9fc526ull, 0x61fffeddfeedaeaeull, 0x53bfffedffdeb1a2ull, 0x127fffb9ffdfb5f6ull, 0x411fffddffdbf4d6ull, 0x11000204000801ull, 0x3ffef27eebe74ull, 0x7645fffecbfea79eull,
};

static constexpr uint64_t BISHOP_MAGIC_NUMBERS[64] = 
{
	0xffedf9fd7cfcffffull, 0xfc0962854a77f576ull, 0x6082209801000ull, 0x3008a08200282080ull, 0x10240420040a0000ull, 0x114412010000000ull, 0xfc0a66c64a7ef576ull, 0x7ffdfdfcbd79ffffull,
	0xfc0846a64a34fff6ull, 0xfc087a874a3cf7f6ull, 0x4002084805002202ull, 0x102824081000030ull, 0x200111041800000ull, 0x4002808220a0e004ull, 0xfc0864ae59b4ff76ull, 0x3c0860af4b35ff76ull,
	0x73c01af56cf4cffbull, 0x41a01cfad64aaffcull, 0x201058818010210ull, 0x4802040420220081ull, 0x4020200a22604ull, 0x41000a00820108ull, 0x7c0c028f5b34ff76ull, 0xfc0a028e5ab4df76ull,
	0x40208400a0084200ull, 0x8024008628804ull, 0x40046610300400c0ull, 0x8104040080401080ull, 0x2488004002000ull, 0x8888018080400ull, 0x4004140809008202ull, 0x242213004807800ull,
	0x101080840411120ull, 0x4012008052409ull, 0x2211149000020401ull, 0x4011400821020200ull, 0x40110040040440ull, 0x14004280141008ull, 0x68020064040908ull, 0x8001020080802400ull,
	0xdcefd9b54bfcc09full, 0xf95ffa765afd602bull, 0x402100443000c200ull, 0x3001002018080101ull, 0x302202220c001200ull, 0x2210010d0a0601ull, 0x43ff9a5cf4ca0c01ull, 0x4bffcd8e7c587601ull,
	0xfc0ff2865334f576ull, 0xfc0bf6ce5924f576ull, 0x8402204108214000ull, 0x2100804042022110ull, 0x8102001020485010ull, 0x8054810010810ull, 0xc3ffb7dc36ca8c89ull, 0xc3ff8a54f4ca2c89ull,
	0xfffffcfcfd79edffull, 0xfc0863fccb147576ull, 0x1140049201462211ull, 0x1d00e00003084801ull, 0x600050050620210ull, 0xc102002102220a01ull, 0xfc087e8e4bb2f736ull, 0x43ff9e4ef4ca2c89ull,
};

static constexpr int ROOK_MAGIC_SHIFTS[64] = 
{
	52, 53, 53, 53, 53, 53, 53, 52,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	53, 54, 54, 54, 54, 54, 54, 53,
	54, 55, 55, 55, 55, 55, 55, 54,
	53, 54, 54, 54, 54, 53, 54, 53,
};

static constexpr int BISHOP_MAGIC_SHIFTS[64] = 
{
	59, 60, 59, 59, 59, 59, 60, 59,
	60, 60, 59, 59, 59, 59, 60, 60,
	60, 60, 57, 57, 57, 57, 60, 60,
	59, 59, 57, 55, 55, 57, 59, 59,
	59, 59, 57, 55, 55, 57, 59, 59,
	60, 60, 57, 57, 57, 57, 60, 60,
	60, 60, 59, 59, 59, 59, 60, 60,
	59, 60, 59, 59, 59, 59, 60, 59,
};

typedef struct Magic
{
	uint64_t magic;
	uint64_t mask;
	uint64_t* attacks;
	uint32_t shift;

	inline uint32_t index(uint64_t occupied) const { return (occupied & mask) * magic >> shift; }
} Magic;

extern Magic ROOK_MAGICS[64];
extern Magic BISHOP_MAGICS[64];

extern uint64_t ROOK_TABLE[0x15c00];
extern uint64_t BISHOP_TABLE[0x12c0];


constexpr uint64_t attacks_bb(PieceType pt, Square s, uint64_t occupied)
{
	switch (pt)
	{
		case BISHOP: return BISHOP_MAGICS[s].attacks[BISHOP_MAGICS[s].index(occupied)];
		case ROOK: return ROOK_MAGICS[s].attacks[ROOK_MAGICS[s].index(occupied)];
		case QUEEN: return ROOK_MAGICS[s].attacks[ROOK_MAGICS[s].index(occupied)] | BISHOP_MAGICS[s].attacks[BISHOP_MAGICS[s].index(occupied)];
		default: return PSEUDO_ATTACKS[pt][s];
	}
}


#endif // BITBOARD_H_DE3A36A4E0B0
