#ifndef ZOBRIST_H_FA2E4D2688B4
#define ZOBRIST_H_FA2E4D2688B4

#include <array>
#include "juicer.h"
#include "bitboard.h"


static constexpr uint64_t PIECE_SQUARE_KEYS[16][64]
{
	{ // CRAP
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	},
	{ // W_PAWN
		0x9cca457523e45153, 0x34c32fe54c418166, 0x891ca45957297deb, 0xfaffcfcea31711ae, 0xa6df81c52307af0f, 0xf2bb0e0e37d2dbe7, 0x4aed336558f61259, 0x56a1870712bb70a0,
		0xe8b95cbce3699aeb, 0xb0299b7ddf48a397, 0x9d64951de48beaae, 0xd478565daf609487, 0xabc287c82c09a282, 0xa8d4d1f4c7f2f304, 0x819bdf21e81bb799, 0x0e1137332acd7fa9,
		0xcaece2ca2c174184, 0x323bb57967846b08, 0xbbcf704674d19e1f, 0x3368f41fd68b7c15, 0x7fc3e0e94af81c53, 0xf0bf3d9a7c8783dc, 0xd7620c06bbe77519, 0x564d23a4938ecb31,
		0x051fb1fc8be1ef53, 0x9bb02f72116dcb94, 0xc3acd7a5241220b4, 0xc5b998be4eae8018, 0x6bc08cbac3f8c40c, 0x188dcaeb6bfba88c, 0x23816ec42d115839, 0xe1e4e3241bd700d9,
		0x27251a941a8c438d, 0x8c7e8f0fa08906d7, 0x8d0c8a4081861ead, 0x0261c62dc37ab1e3, 0xfc28e33685b29884, 0x02c660a9927c6c0c, 0x4bc3014258e0a47c, 0xfdb4c2bd1d48fb12,
		0xff56b71ff55848f7, 0x11292a4c9ecdd207, 0x973b887da2e64d9e, 0x92ceb1b1c39674a0, 0xf26ac38e36f4d026, 0xf912f74fc7de82a2, 0xda02e9eb088e728f, 0x15a789d91b829b89,
		0xf1993cff590c586a, 0xcc02285732a6c16b, 0xbefadabde02bcdcb, 0x1ba3657c054e3bdb, 0x6f97246d7d9646fa, 0x0d1a498d07e7ced5, 0x07c09d346e0e7ea6, 0x7dc109994d6a3a7c,
		0x13ee2fb8c1cd4af0, 0xec8078c747da3f4e, 0x65cc780fa0786928, 0x8170ac4ac5512abb, 0xd932fc67b59c6b8f, 0xdc0878fe509d7a17, 0x8d4d26f16e1fc844, 0x6825d92bc2eed71e,
	},
	{ // W_KNIGHT
		0xc4342fa99a20a2bf, 0x279ce2613d4aebf1, 0x0b0fb6781a10ac0f, 0x8502b5e366054571, 0x3a35f87fb34c46b3, 0x6463a170e0666090, 0x4a9dbe442e29bd94, 0xf26f0164ea25ba02,
		0xa99fe422bfffd080, 0xb23aaee088e9b581, 0xc7e75460893d672f, 0x9ddc1729d8572722, 0x01ab51752e5f748f, 0xacb1113b0f8a6b70, 0xbf127a315791f147, 0xa20c37f573556cd2,
		0xc0a1c4430952975e, 0x562149677fd1d2b0, 0x10db1d3e3b4f5f95, 0xc2ea738a706c71da, 0xa0507a6c6ffc95f6, 0x34d8c491d11b5801, 0x43440d78d7b5d506, 0xaebadb15beb2b149,
		0x5176954d90e77d4c, 0xb3a12a6821f71cfb, 0xc462306641700342, 0x945f63aad41329c5, 0xc2cd8ac5315bb923, 0x5a780b6c14ffdde4, 0x5da4790473dd455c, 0x14b0d17549df2afe,
		0xff89210058ba475e, 0x72b0632d34d43b6f, 0x3abc3b200657249b, 0x1b69c620a3648174, 0xfba10a79e60f55ba, 0xb763eba47ad38feb, 0x375993531c79384a, 0x7e2f75710a33bc91,
		0x06c1580314e1ca9e, 0x1d7158cfca0ef896, 0x633aa6d1d17764ca, 0x45e694a29a227d64, 0x22a53f4b16715358, 0x63c9b35a8f4ff313, 0xd6a717110665ab0a, 0xe9dacf089e837507,
		0xd9bb8d26829cbf6c, 0x23c59b3fed9bfa2e, 0x04aa11adf269db79, 0xab17e1e5c2ddd408, 0xbc4824a416171b4f, 0xc36fc6b8a165cbc3, 0x8efabe2e032b8abd, 0x99f0dd84b7d32fd4,
		0x48b3e01267dca8c0, 0x42fc4a17f694bb31, 0xd50a81333c34581e, 0x6b6ec4de9f280785, 0xb032450170c19099, 0x0389d4b1b3aa747d, 0xcd266693ac30ceda, 0x88d582daa2db2979,
	},
	{ // W_BISHOP
		0x3c3a4983a379ed2d, 0xfd8c3dac7770de7d, 0x1a554b8b211059ed, 0x9b2eabcd40de86d2, 0x3210054d0f092d16, 0x477ec54f6e470192, 0xaf33f81c70b35b16, 0x4870e0cfef682fb2,
		0x9a57dd7443d9c425, 0x8120753a3d3b8208, 0xc371b69d1136d55d, 0xe5998313dd7186bb, 0xd8d8af1f8db3d893, 0xb83eb55ed25cfd63, 0x136243d7d5b596d3, 0xe0d5b9d8e1acfb6a,
		0x552848cc2dc66c37, 0x3e198d9a42631be5, 0x1f3002700b20e5fc, 0x460cf7a26ecdd5fa, 0xbf3c98ad7f9583c3, 0x380bece39ddecc16, 0x6a2751e7a1ea7d08, 0x5e15e600172fc4a6,
		0xb0562ec088e30ee2, 0x21bd2728b3776e67, 0x23038ac290d4bfef, 0x1b3f02a80e537a9c, 0x472e82d431d51c71, 0xbccd2c1366521f89, 0xd708378047490e11, 0xec5a5648262e186a,
		0x20668f659cee221a, 0xef126a6883ef00d8, 0x74299dcd8670560f, 0x79ed4991ae150e3d, 0x60c28bf9eeb0139c, 0xc2ae797bb8ea30c1, 0x573f3601702a35f9, 0x306d4a70913c1a86,
		0x20ef0d2bfd118089, 0x16968efe18a84c4c, 0x65263babc78d9770, 0x3afc55c61055fd87, 0x3e0494a46674feb9, 0xe92ff81740378177, 0x9fefa97e83972704, 0xa43fa5ef44afc414,
		0xb94ebde39f0b2e84, 0x4e7795e0b1ddd18f, 0x635a5211cf7ba3ce, 0xb34bc1a7e68e9e65, 0xc57a68c6bc611c42, 0x5b4234e5223f0bd0, 0x4bc27c889414de15, 0x80b97945c8114d34,
		0x0519b68bd74cc5ef, 0x39799f9ac13ac67d, 0x9740942c77af8004, 0xc2575e76fcd7550e, 0xc371b49dfaae288b, 0xaf0f026b967bc7e2, 0x00a22c4595d974e7, 0xea24a9c6ba280ad3,
	},
	{ // W_ROOK
		0x090e83877780d2f2, 0x6bc32b349d04287b, 0x9710272ebcabe85b, 0x0fe71c4cd72d8b8a, 0xf198ff260228cf3a, 0x1b4eac56a36aa532, 0x99b4e130852dc309, 0x64500f226bd08062,
		0xb94e42063ac1d72a, 0x890863fd7310325a, 0xb84836d5e87e9342, 0xed2df519347ecc8d, 0x580f4fdd885d1fa5, 0xfba94c9b6672b80f, 0xbc5a9256202d2e93, 0x9bda17fd6541a4f9,
		0x29392a9660d1e9ba, 0x43f432a21ef84425, 0x6dcf4d7490fd39ac, 0x4d3ed85db57529ad, 0x23228961a733b7e5, 0x52c659e89f7ad9fd, 0x73e0831c3f7747a6, 0x36e8f10837d1aeca,
		0x76d2268b544bae0f, 0x65059a8b0f59b0ea, 0xac8a2eb4e5a75d99, 0xb553d09823e031b5, 0xbf3c6a891d9fbffa, 0xfef2da2dd2c804bd, 0x6cf08ef95c415c2b, 0x382820983e0112fe,
		0x79da225abefe3a3f, 0x4dca8c87a0fb6bc9, 0x42288a7e4a376530, 0x3aa0d8d9fda62d9e, 0xad5e4525c8b80e63, 0xd1c23f4b52b3e735, 0x9231258b0c097d74, 0xcfdfa46b5bb434d3,
		0x6661738d85fdbae6, 0xa4e1f0f8ccf889a6, 0xd1e7424872474c62, 0xd842aa6504787266, 0x6f3b1e158eadafd0, 0x0e16ea69b354e457, 0x8dedbd5a64df28e8, 0xc314f5a153db4c92,
		0x902607adbacc8121, 0x387b760f6ba15db9, 0xc8b3a62781245b26, 0xc81aafe1d59a7980, 0xdd2701469dbf8ab9, 0xf31a88106808438e, 0x139a674a3083f53c, 0x7b84b0789ecaed17,
		0x31f49c381858e30a, 0x481a947dbe000042, 0x4e7e391db6d2e489, 0xa2836d066ce377dc, 0xa20ec54295660b31, 0xc4bf2bfb43b72c25, 0x680248fa2b6eac11, 0xe82d0de6417ac43f,
	},
	{ // W_QUEEN
		0xc8646646f7add728, 0x9c95dd910f6f6235, 0xfe2c657b180d94c9, 0x5d1e1c41928a21da, 0xe2a12214b5e51c95, 0x3ae1a015b0007d93, 0xea25b53735a72408, 0x3720e93198559c23,
		0xb5a4801a73b70968, 0xb9755f502c694a27, 0xf3474be9559185ed, 0xd356a937093b2d0d, 0xc1d799c3c956344b, 0x595be67bae97a52a, 0x9ac59e4359d5f241, 0xc3da1878b8e68031,
		0xac73a0d24e418ad4, 0x42a59cf2a38ec2a1, 0x910049a31545ad6c, 0xa058156835315b4a, 0xb99b5a5b28f1dcf0, 0x47d35ad775ce2db2, 0xabc287274670936e, 0xae0611d22f9cdb5c,
		0x645b4561cecd8a44, 0x027635d6a49c0739, 0x74349fc420a56bc3, 0x35ef7ba9dba69283, 0x98b08b8d10e3e4d4, 0x66b7bed546a15da2, 0x27b002d30c12652c, 0xd97a8eb31e838ac9,
		0xf8d2b8cf04b8346b, 0x2396da66ec4c2f89, 0x6cc7d8c68ecac9e2, 0x6bdceaf21b5a3257, 0x61f14ffeecf9c199, 0x922d2fbbfe74fa09, 0xcfb06ae3f7805b78, 0x585d916f63f52326,
		0x20049711655f019b, 0xf03f781b2c801fa0, 0x383f70743442a269, 0x31a62c5f269b3694, 0x7f419a54b8558081, 0xfab41d6513740e22, 0xbbaccec983b642da, 0x9696aaaf6aea4006,
		0x781bef245a2557de, 0x29dc2a7189665062, 0x809ec13ab1c999c4, 0x070e8b9bdb55116a, 0xe7dc5bbfa1d3d868, 0xbbd49dd9ec961427, 0x3586127389a65650, 0xdb224596cb70d782,
		0x690a8126d110f3fa, 0xcc5f76dd860366d5, 0x0741cb3bbda505b3, 0x37b3d39fd5d3f033, 0xef73b587bdc9ff0c, 0xc530c2853057a65e, 0x32fe16ab1d04bbf0, 0x8badf92bc5b88388,
	},
	{ // W_KING
		0x92d4e27ebaf5d17d, 0xc98a9e635d2c5572, 0x47a0ad550e3dbe3c, 0xf9eb97294b8071de, 0xcf006a4c84806dbb, 0x9d6eef346b4efc61, 0x1adfd3d0d2043b4b, 0x13513ce6cfabe928,
		0xc423b3029bb2ed70, 0x5d5b051e20a5a0e9, 0x3d7c80313f8296c5, 0x4cf869f86daaf582, 0x38bb311485526a47, 0x865199b0bea11ffe, 0x7bc12d968eeddadb, 0x6fe3fed60c81506a,
		0x09eca922f2b37df4, 0x5149790a038f4f3e, 0xb2686a74ae7d3594, 0x3b8f3fa71a00399d, 0x62201a8f923f3097, 0x9baa1b10f9ec3e6e, 0xf34690176287ccc1, 0xb1163924f288fa3e,
		0xe24845df53293c6a, 0x128db15cd14ef1c7, 0x5af081fa0e955df1, 0x840455068b5bc045, 0x993875bf415bd030, 0x8a6b338a6d2a667a, 0x38c2a8922c2eeb89, 0x1edc91b1d4cad636,
		0xe1b0f7d2410b2fcf, 0x807ed4de3a54883b, 0xad7caa25deea5b4e, 0xab087ba07e6bb8c3, 0x41c0707cda39980f, 0x2dce62ba047198be, 0x8d734c2448cc3349, 0xcd0fc50ba95774db,
		0x6f5bc7127e7af83e, 0x4cdd5c8b305adae4, 0xcf82f42fc269ba9e, 0x177970a511f18384, 0x0cabc158c607e4c7, 0x2577a9ae906b8150, 0x5cc176ebd46a72f0, 0x17fa1eb589e87ed3,
		0x52be2edba9b0c8be, 0xfb5b6c47301c43d9, 0xc5ac84b2a208c42a, 0x307450eacf4a8aed, 0x9a4d4dba005f1732, 0xab3aa203593547b5, 0x1671bbd44177e0b0, 0x42558366798319dc,
		0x20b71dc3d8049fed, 0x586283510aadf211, 0x114dd7162cef70d5, 0x882d9fb2ef340851, 0xaaba29e9138e616b, 0x1127dcd835cd54a5, 0x0fcd4df90e44b1df, 0xdd52845605833272,
	},
	{ // CRAP
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	},
	{ // CRAP
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	},
	{ // B_PAWN
		0xa349d48e54a97bba, 0xb3c74f05132e74f2, 0xf8703105cfa39be3, 0xa9f479cc24995d9b, 0x3b1a4809588f63c6, 0x4d613f057f5f9a62, 0x0325a9cb88a721ae, 0xcba66f574405dd50,
		0xfd4f584aa06d9270, 0xc42ca496f3fa6453, 0x2e90431a9cb4b7de, 0xbef6fe5aff08b258, 0x24e82ab3959f47db, 0xbd612701dad06d75, 0xfee4f0c8dc30dc39, 0x947147877c36cce8,
		0xcd8915c84238ca70, 0x6fdab7f56e0dd567, 0x2370572f59b118e1, 0x26d2441e419bb9b2, 0xcaf3fe9019ce053f, 0x2265162b95017275, 0x53314d5c5c204efc, 0x63410af0bbceaee6,
		0x134ce5eceb0ed43d, 0xa02a3688f4a8109b, 0x52391d98c533f55b, 0xdcc3021566c78158, 0xa207aadc1dc3e703, 0xc16e5465dd3d3c0e, 0x5ed2e705e1e673ff, 0x4012292986458ba4,
		0x9b423c260fba03d5, 0x9dfc1eea6d507f57, 0x28d42cf807718a31, 0x64d836e2df7cd4bd, 0x8572de835e2bc0af, 0x2ca72abc206feade, 0x232e875a6ea411f4, 0x4f3dad9a905b654c,
		0x4205457b036e4724, 0x66e3a07ea091e31d, 0x83af4d683fb69d0a, 0x2d5c9e6811f1396f, 0xfb256ec3008577d8, 0x33ab246fd5e428fb, 0x87a1d7faf1074d9b, 0x579f9c0d41513728,
		0x9778243b495afcdd, 0xab3c4f0f83662b04, 0xef768d4a027a7564, 0xf4574eb24601268a, 0x6b3d85332732e0b7, 0x4e428d295cdcb5c4, 0x2b5de80b3be8dc52, 0x037b6464a5d2fde5,
		0xf89c183cc773c6bd, 0xaa028188b7f60568, 0x75d432ce6e567a90, 0xe9390101c8b0f22e, 0xf2c4aad162c11633, 0xccd7973bb8cb1abc, 0x2c8afc09f93149d7, 0x4fcac17248aa12ee,
	},
	{ // B_KNIGHT
		0x076810fa0270964a, 0x4b9c7f37d127a75e, 0x37e9165d70644420, 0x74e47644a2b2a58e, 0xb7b6de4352a9a3d9, 0xfa2f4385892e2ae7, 0x0085d4bd0e5843d6, 0x6f2278c2fb564fd0,
		0xd1b9df45f8b1dda9, 0xaa843c9beb37eba2, 0x539dd35ee27aff44, 0x4bab5e64d63c0e69, 0x9b811cca22069cf6, 0xc3ed53ed58918b13, 0x648b28ade96740d6, 0x903eb329ce789b0c,
		0x9d9d49be91d343b5, 0x9ea0f40128517a71, 0x57e76fd3b0d1220b, 0xef9e0b583ea795d2, 0x5adf97a096815ddc, 0xd0ff459f7ef95205, 0x68c6d323a00459ac, 0x37234662fb3c3429,
		0xb170ca03db7a9251, 0xbba9630aa1b04abb, 0xa5fbd4198f0e2ea4, 0xc6a876ff8a43be59, 0x056a951feefc309d, 0xb36364ee5a803333, 0xb74852f56b5aed8a, 0x62760498b5091644,
		0xe6788ccd87ad53f1, 0xd33a0195374727f7, 0xfeef3d650d4ac59e, 0x84942256f5b4eb49, 0x01b52d6ffe9daeac, 0xfa3cf21a5c56bfcc, 0x75181ea8933189e7, 0x1ba6f66ea5b75fd0,
		0x6efc4bb6eb4aa142, 0x538aa44f7dd372f9, 0xf4a43ab98ae903f7, 0xfd045ccbdcf7fcc7, 0xac6139a23fc14130, 0xdf2dbe8239118fa7, 0x504c6b140fcd2b88, 0xd34026c92b310628,
		0xc765805b3840cabc, 0xaa2206662df7019d, 0x5a142a288bfcfb5d, 0x717cb7444416dd40, 0x568e7869c1f8cd07, 0xdac87a9687c18e95, 0x99c850e2f9d857cf, 0x3a8efb64ec95fa56,
		0x51564650c6ac7c02, 0x3e0bff5858f6a261, 0xdc5e80da9328af6f, 0xfaa6a582a1707f5b, 0xfcb859096c6c6205, 0xc471b2017dd29a91, 0x14f0e37fec80db0e, 0xcb8984d32393d36f,
	},
	{ // B_BISHOP
		0xdbc9d7d8d8532f46, 0xf92f101277801264, 0xc1fefad21cc63c38, 0x4f763a403ba5840a, 0xe6f9512a60989eba, 0x2f736a95759500fe, 0x4babd34be574fbd1, 0xd309f47112586261,
		0xff590f87e80612b4, 0x451e35b010e632f6, 0x63268b00e06253e4, 0x521837c588d4c8f7, 0x18071c7030200d8a, 0xf129918da4b99bae, 0x0127425e887343fa, 0x4feb35fb39c3edde,
		0x465bfa7421fb5dd9, 0xc25a6b9cf05f6116, 0xf424e1a5c76d992f, 0xe8aebd818f4140ea, 0xab49f077881377ea, 0xeadaf1871faacbff, 0x6bb7fe5e639429d1, 0x94634585adc5392d,
		0x404478398540b9d7, 0x48c8a35380578cfa, 0x85affee97e063a01, 0x5ff9aa7fdb000d37, 0x553c95d093de62cd, 0xa9412a5b0bf42bd6, 0xb4732c012cfb6f3e, 0x90ffdc6932d98afd,
		0xd7acbe920501b251, 0xd11abb3dc07af05d, 0x1ad1bd9cf40fabbe, 0x0e528c93a63dea7c, 0x7576bbdf250769fb, 0x8d2fd25654d76b64, 0xbaa239690166adbc, 0xb44f19bbd4f19037,
		0x1099d216e26077df, 0x72aaafbc6e57322e, 0x54d0225265aa00fd, 0xd056f67bcb535ba6, 0x589f9be32b014af5, 0xf8a05c6c78f69fd0, 0x223d5013ec3f20f0, 0x26bf9cf3148c2768,
		0x66d07e11eb49b49b, 0x47215b899aa45e9d, 0xca62fbb49032d5a4, 0x6d5d4c5a86a35262, 0x54db36707ae3f6f6, 0x85c6f5e74c0cc014, 0xa7337310b6cc1ea9, 0x094372f8b81091bc,
		0x0808ba702cf2c04f, 0x5270dbb5af679889, 0x4df69cae6289e2f1, 0x1cb12100585f9167, 0xea406d7f34fa6ad9, 0x9e4f5f14d8d0a4ac, 0x8ad46f191fcc9786, 0xe71588c8d9693b70,
	},
	{ // B_ROOK
		0x3f496a9eda272d7f, 0xe81b95c3668d5743, 0x0b1e95c3b03d6003, 0xb828f3f3e14608c0, 0xc6b8b81ba5443f69, 0x0dc9515dc892d3cd, 0x7f2805752b7cbe78, 0xfc07d30ec176a173,
		0x429eaaa9ecb39310, 0xb270a404590faf04, 0xe979f20aa4157422, 0xc47fe61e9fca292d, 0x8b936344da2c89fb, 0x459867bd7c144048, 0xcd12724a774c8abd, 0x68736c7be4be58f0,
		0x109ff8e7f6350418, 0x4b087ce491955205, 0xdc7c578cc06ad46d, 0x56ba1a14de298d38, 0x3821abe0f32af5cd, 0xa30482de13638817, 0xf9ad2846323249d1, 0x4beb017ad00abda1,
		0x51e54c09fa22b792, 0xad91d1dc49bd8b69, 0x0641a3549fd51e8e, 0x63cecd2d6f4df23c, 0x14c0c9003234c05c, 0xb1baa0abe88b4579, 0xa98562aff9fa6183, 0x8a4662c5b7b8ee52,
		0x15def69a1af7a77b, 0xe848000b3acabba1, 0x6de317c88abc10d0, 0x6756e774ff3c0eba, 0x00cbd4c881adbf04, 0xa7711d3746810e16, 0xdc8c4d51922831d1, 0x15106cdfcef6add5,
		0x66eacd145b3dd04e, 0xb215d6774bbe23e2, 0x27a6b6e3932bb648, 0x74ebbed88abd5610, 0x0166d516d33f776f, 0x8cac992061fa468a, 0x458066e3782ae66d, 0x5bd22dcc183de93d,
		0xa088f00af1411be8, 0xd1f78cc2d7465185, 0xf588a18502331c13, 0x31c0178f7918e3e6, 0x4ce3a977d62ed982, 0x4e83126d0ef60384, 0x7b8fb6c22d016bde, 0x709b8403c2a7cc00,
		0xd45d8bf5e00e5f4e, 0x2dc16495f87e88ec, 0x0c7349ccf8198800, 0x9acbba8a328a5d75, 0x62bfadb42685d7ae, 0x20507ef3ebd39109, 0x8aae4dca7e8196a2, 0x232569e163c725af,
	},
	{ // B_QUEEN
		0x7d8318aea6deaa25, 0x9f81579502596afa, 0x820032f52586fcb4, 0x98951ad1cfd2b154, 0x905cca867e28c572, 0x2d57fa0bf6ad6f13, 0x62ba2d9e1c77c8c2, 0x2551cccc4c45b67e,
		0x3f2a3a71e4e0292e, 0x39b3ec6718d18405, 0x6140ce71841153db, 0xb19587e9b48655e2, 0x72eb02070e712994, 0x3b25bac3a6d2f648, 0x492aa86692d31681, 0x9c5dff4722cdaf01,
		0xabfbab7fdbcbad40, 0x20c3d4426df46414, 0x6794f966fa2a7a99, 0x66c4a3f672ce5d42, 0x17a961c83e71c10c, 0x064fe4a1837cb619, 0xaa9cb13d23c7d5a2, 0x23d7aedfedb755ba,
		0x4f8adc3eea352713, 0x2025997d5b7f9d1e, 0xa4d6a76282dc3ff3, 0xe92321969d76e47a, 0x56a60dc347b6c2c4, 0xc7bce8ae3103aa8a, 0x0521db2b3eb069d4, 0x2b820acf8a97abc5,
		0x106571abb2847e16, 0xdd567d1e8d69a991, 0x3f48c81a6cc84eb0, 0xc8c078bb6dd9974a, 0x43e9345c16cb3127, 0x2d36b87604bd4fd2, 0x9ac2d3b3b7503ff6, 0x1ecfa4094c766fa7,
		0xcd71de8bb474a2b2, 0x71fc89589cc52fd8, 0xcaaa65ecdcda81e8, 0xd13f2cce5035e89f, 0xa07eaeda5745929f, 0xbe4f7c0b6d38bd0d, 0xeb3b93c1b352b41f, 0x84d59cb6eabe0258,
		0xaee427079ce14caa, 0x01240a81b094a7cc, 0x97bdc288dececa1d, 0xedc5f0f0ef94d3f5, 0xfb5802654a0520af, 0x0a908972cebad316, 0x6697254586040fb1, 0xc80615ee671f87a0,
		0x2afa5a85acb3dabd, 0x83987c1f104924e7, 0x0e761dc432270002, 0x2febb56f6b212281, 0x39775a51bb0dd430, 0xef9e44944a4fb3b5, 0x8038ad2bd8f436d5, 0xca9f957503a4356d,
	},
	{ // B_KING
		0x0ff84737db39a00c, 0xe32355e4786067f1, 0xda4f62be43b0e5f3, 0x3e97a2c5133d8e93, 0x0c7bc40c1fbfe3b1, 0x201dd9dae88350bf, 0x04e1bd4591ea4f23, 0x5b61037a4cd7d9ee,
		0x2f075c10fc864790, 0x784ec4465d63a750, 0x18ef3ed77f2b3483, 0x132cb85ff2855d05, 0xe957c029658b54b0, 0xb70bb0e98e1580cb, 0x64d90327987622e5, 0x8bb237eeae85f33d,
		0xc85d7e7aca466700, 0x541ca643cba15d7d, 0x442b702de71ade69, 0x44b9efa83ab0c767, 0xffb8fb7b08c22fa7, 0x7b2210ad37ab199d, 0xa3031a658fa95641, 0x6cb6f7e2abe36bc5,
		0x301680aca445b6d1, 0x728e4694f1c10c61, 0x3be7c38c8c3e20ce, 0x3408c81b1bd94ba7, 0x738c5d3791c8b157, 0xab7259121d1d712f, 0x3e9470355ae81534, 0xb9ff7d6e2d055b20,
		0x3c5369faf07948a9, 0x37537adf35ef4e69, 0xd8777d49b532d62c, 0xe44dc230f32ca32b, 0x4b0754691e4233f2, 0xf0d6c9180d2958fe, 0xa5c432bf174f4015, 0x537603c61c96ba9d,
		0x28b3a62be2851c7b, 0x50d73505fcb230a0, 0xf43e50f954bc200f, 0x9b6698a2908c6e9c, 0xe137f16ac4fac910, 0xac71b4477f64c0fd, 0x13311470271c78b2, 0xf9903496be01c7ba,
		0xf29310f08423aced, 0x7c74cd4de41198e1, 0x32790673ea076652, 0x118c76370b469ec3, 0x1a4e3ff458d58239, 0xabcc0f0698801c1a, 0x36debb2447663954, 0xd11285701e3abf90,
		0xbfbcf3822d91ab8d, 0x614bdd5124b98e5e, 0x50fd4d5643bde147, 0x37ae0d04585969aa, 0xe1aa6dff2ccd180c, 0x7ddad0170bce8331, 0x30f08a02cc3b37b4, 0x029a82113164c956,
	},
	{ // CRAP
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	},
};

static constexpr uint64_t TURN_KEY {0x1128e7c61d9c201e};

static constexpr uint64_t W_OOO_KEY {0x573a9911cb53ed19};
static constexpr uint64_t  W_OO_KEY {0x3091173ea4210715};
static constexpr uint64_t B_OOO_KEY {0x1a4be03303fd5b4a};
static constexpr uint64_t  B_OO_KEY {0x39f1ecd65889b67d};

static constexpr uint64_t W_CASTLES_KEY {W_OOO_KEY ^ W_OO_KEY};
static constexpr uint64_t B_CASTLES_KEY {B_OOO_KEY ^ B_OO_KEY};

static constexpr uint64_t EP_FILE_KEYS[9]
{
	0x1531775bd4f0ae39,
	0x938917a4580dab26,
	0xa109c3f8edb9dc4f,
	0x3d96e0c0dff3da03,
	0x589d742f8997a5db,
	0x0fd279c829a37021,
	0x251c6274cf0fb728,
	0x947c0c25384c250f,
	0,
};

static constexpr std::array<std::array<uint64_t, 64>, 64> W_PAWN_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[W_PAWN][s1] ^ PIECE_SQUARE_KEYS[W_PAWN][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> W_KNIGHT_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[W_KNIGHT][s1] ^ PIECE_SQUARE_KEYS[W_KNIGHT][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> W_BISHOP_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[W_BISHOP][s1] ^ PIECE_SQUARE_KEYS[W_BISHOP][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> W_ROOK_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[W_ROOK][s1] ^ PIECE_SQUARE_KEYS[W_ROOK][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> W_QUEEN_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[W_QUEEN][s1] ^ PIECE_SQUARE_KEYS[W_QUEEN][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> W_KING_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[W_KING][s1] ^ PIECE_SQUARE_KEYS[W_KING][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> B_PAWN_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[B_PAWN][s1] ^ PIECE_SQUARE_KEYS[B_PAWN][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> B_KNIGHT_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[B_KNIGHT][s1] ^ PIECE_SQUARE_KEYS[B_KNIGHT][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> B_BISHOP_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[B_BISHOP][s1] ^ PIECE_SQUARE_KEYS[B_BISHOP][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> B_ROOK_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[B_ROOK][s1] ^ PIECE_SQUARE_KEYS[B_ROOK][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> B_QUEEN_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[B_QUEEN][s1] ^ PIECE_SQUARE_KEYS[B_QUEEN][s2];
	return arr;
}();

static constexpr std::array<std::array<uint64_t, 64>, 64> B_KING_KEYS = [](){
	std::array<std::array<uint64_t, 64>, 64> arr {};
	for (Square s1 = A1; s1 <= H8; ++s1) for (Square s2 = A1; s2 <= H8; ++s2)
		arr[s1][s2] = PIECE_SQUARE_KEYS[B_KING][s1] ^ PIECE_SQUARE_KEYS[B_KING][s2];
	return arr;
}();

static constexpr uint64_t W_OOO_UPDATE_KEY {W_KING_KEYS[E1][C1] ^ W_ROOK_KEYS[A1][D1] ^ W_CASTLES_KEY};
static constexpr uint64_t  W_OO_UPDATE_KEY {W_KING_KEYS[E1][G1] ^ W_ROOK_KEYS[H1][F1] ^ W_CASTLES_KEY};
static constexpr uint64_t B_OOO_UPDATE_KEY {B_KING_KEYS[E8][C8] ^ B_ROOK_KEYS[A8][D8] ^ B_CASTLES_KEY};
static constexpr uint64_t  B_OO_UPDATE_KEY {B_KING_KEYS[E8][G8] ^ B_ROOK_KEYS[H8][F8] ^ B_CASTLES_KEY};


#endif // ZOBRIST_H_FA2E4D2688B4
