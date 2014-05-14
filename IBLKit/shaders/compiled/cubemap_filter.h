#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer CSBuffer
// {
//
//   float4 zDirection;                 // Offset:    0 Size:    16
//   float4 xDirection;                 // Offset:   16 Size:    16
//   float4 yDirection;                 // Offset:   32 Size:    16
//   float4 texelOffset;                // Offset:   48 Size:    16
//   float4 roughness;                  // Offset:   64 Size:    16
//   uint4 uavSize;                     // Offset:   80 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// smIBL                             sampler      NA          NA    0        1
// txIBL                             texture  float4        cube    0        1
// txOUT                                 UAV  float4     2darray    0        1
// CSBuffer                          cbuffer      NA          NA    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[6], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texturecube (float,float,float,float) t0
dcl_uav_typed_texture2darray (float,float,float,float) u0
dcl_input vThreadGroupID.xy
dcl_input vThreadIDInGroup.xy
dcl_temps 9
dcl_thread_group 8, 8, 1
resinfo_indexable(texture2darray)(float,float,float,float)_uint r0.xy, l(0), u0.xyzw
imad r1.xy, vThreadGroupID.xyxx, l(8, 8, 0, 0), vThreadIDInGroup.xyxx
ult r0.zw, r1.xxxy, r0.xxxy
and r0.z, r0.w, r0.z
if_nz r0.z
  ieq r0.xy, r0.xyxx, l(1, 1, 0, 0)
  and r0.x, r0.y, r0.x
  if_nz r0.x
    mul r0.x, cb0[4].x, cb0[4].x
    mad r0.x, r0.x, r0.x, l(-1.000000)
    lt r0.y, |cb0[0].z|, l(0.999000)
    movc r0.yzw, r0.yyyy, l(0,1.000000,0,0), l(0,0,1.000000,0)
    mul r2.xyz, r0.yzwy, cb0[0].yzxy
    mad r0.yzw, r0.wwyz, cb0[0].zzxy, -r2.xxyz
    dp3 r2.x, r0.yzwy, r0.yzwy
    rsq r2.x, r2.x
    mul r0.yzw, r0.yyzw, r2.xxxx
    mul r2.xyz, r0.zwyz, cb0[0].zxyz
    mad r2.xyz, cb0[0].yzxy, r0.wyzw, -r2.xyzx
    mov r2.w, l(0)
    mov r3.xyzw, l(0,0,0,0)
    loop 
      uge r4.x, r3.w, l(0x00010000)
      breakc_nz r4.x
      utof r4.x, r3.w
      ushr r4.y, r3.w, l(16)
      bfi r4.z, l(16), l(16), r3.w, r4.y
      ishl r4.y, r4.y, l(1)
      bfi r4.y, l(16), l(17), r3.w, r4.y
      ushr r4.z, r4.z, l(1)
      and r4.yz, r4.yyzy, l(0, 0xaaaaaaaa, 0x55555555, 0)
      iadd r4.y, r4.z, r4.y
      ishl r4.z, r4.y, l(2)
      ushr r4.y, r4.y, l(2)
      and r4.yz, r4.yyzy, l(0, 0x33333333, 0xcccccccc, 0)
      iadd r4.y, r4.y, r4.z
      ishl r4.z, r4.y, l(4)
      ushr r4.y, r4.y, l(4)
      and r4.yz, r4.yyzy, l(0, 0x0f0f0f0f, 0xf0f0f0f0, 0)
      iadd r4.y, r4.y, r4.z
      ishl r4.z, r4.y, l(8)
      ushr r4.y, r4.y, l(8)
      and r4.yz, r4.yyzy, l(0, 0x00ff00ff, 0xff00ff00, 0)
      iadd r4.y, r4.y, r4.z
      utof r4.y, r4.y
      mul r4.xz, r4.xxyx, l(0.000096, 0.000000, 0.000000, 0.000000)
      mad r4.y, -r4.y, l(0.000000), l(1.000000)
      mad r4.z, r0.x, r4.z, l(1.000000)
      div r4.y, r4.y, r4.z
      sqrt r4.z, r4.y
      add r4.y, -r4.y, l(1.000000)
      sqrt r4.y, r4.y
      sincos r4.x, r5.x, r4.x
      mul r4.w, r4.y, r5.x
      mul r4.x, r4.x, r4.y
      mul r5.xyz, r2.xyzx, r4.xxxx
      mad r4.xyw, r0.yzyw, r4.wwww, r5.xyxz
      mad r4.xyz, cb0[0].xyzx, r4.zzzz, r4.xywx
      dp3 r4.w, cb0[0].xyzx, r4.xyzx
      add r4.w, r4.w, r4.w
      mad r4.xyz, r4.wwww, r4.xyzx, -cb0[0].xyzx
      dp3_sat r4.w, cb0[0].xyzx, r4.xyzx
      lt r5.x, l(0.000000), r4.w
      if_nz r5.x
        sample_l_indexable(texturecube)(float,float,float,float) r4.xyz, r4.xyzx, t0.xyzw, s0, l(0.000000)
        mad r3.xyz, r4.xyzx, r4.wwww, r3.xyzx
        add r2.w, r2.w, r4.w
      endif 
      iadd r3.w, r3.w, l(1)
    endloop 
    div r0.xyz, r3.xyzx, r2.wwww
  else 
    iadd r2.xy, r1.xyxx, -cb0[5].zwzz
    itof r2.xy, r2.xyxx
    itof r2.zw, cb0[5].zzzw
    div r2.xy, r2.xyxx, r2.zwzz
    add r2.xy, r2.xyxx, cb0[3].xyxx
    mad r2.xzw, cb0[1].xxyz, r2.xxxx, cb0[0].xxyz
    mad r2.xyz, cb0[2].xyzx, r2.yyyy, r2.xzwx
    dp3 r2.w, r2.xyzx, r2.xyzx
    rsq r2.w, r2.w
    mul r2.xyz, r2.wwww, r2.xyzx
    mul r2.w, cb0[4].x, cb0[4].x
    mad r2.w, r2.w, r2.w, l(-1.000000)
    lt r3.x, |r2.z|, l(0.999000)
    movc r3.xyz, r3.xxxx, l(1.000000,0,0,0), l(0,1.000000,0,0)
    mul r4.xyz, r2.yzxy, r3.xyzx
    mad r3.xyz, r3.zxyz, r2.zxyz, -r4.xyzx
    dp3 r3.w, r3.xyzx, r3.xyzx
    rsq r3.w, r3.w
    mul r3.xyz, r3.wwww, r3.xyzx
    mul r4.xyz, r2.zxyz, r3.yzxy
    mad r4.xyz, r2.yzxy, r3.zxyz, -r4.xyzx
    mov r5.xyz, l(0,0,0,0)
    mov r3.w, l(0)
    mov r4.w, l(0)
    loop 
      uge r5.w, r4.w, l(0x00010000)
      breakc_nz r5.w
      utof r5.w, r4.w
      ushr r6.x, r4.w, l(16)
      bfi r6.y, l(16), l(16), r4.w, r6.x
      ishl r6.x, r6.x, l(1)
      bfi r6.x, l(16), l(17), r4.w, r6.x
      ushr r6.y, r6.y, l(1)
      and r6.xy, r6.xyxx, l(0xaaaaaaaa, 0x55555555, 0, 0)
      iadd r6.x, r6.y, r6.x
      ishl r6.y, r6.x, l(2)
      ushr r6.x, r6.x, l(2)
      and r6.xy, r6.xyxx, l(0x33333333, 0xcccccccc, 0, 0)
      iadd r6.x, r6.x, r6.y
      ishl r6.y, r6.x, l(4)
      ushr r6.x, r6.x, l(4)
      and r6.xy, r6.xyxx, l(0x0f0f0f0f, 0xf0f0f0f0, 0, 0)
      iadd r6.x, r6.x, r6.y
      ishl r6.y, r6.x, l(8)
      ushr r6.x, r6.x, l(8)
      and r6.xy, r6.xyxx, l(0x00ff00ff, 0xff00ff00, 0, 0)
      iadd r6.x, r6.x, r6.y
      utof r6.x, r6.x
      mul r6.y, r6.x, l(0.000000)
      mul r5.w, r5.w, l(0.000096)
      mad r6.x, -r6.x, l(0.000000), l(1.000000)
      mad r6.y, r2.w, r6.y, l(1.000000)
      div r6.x, r6.x, r6.y
      sqrt r6.y, r6.x
      add r6.x, -r6.x, l(1.000000)
      sqrt r6.x, r6.x
      sincos r7.x, r8.x, r5.w
      mul r5.w, r6.x, r8.x
      mul r6.x, r6.x, r7.x
      mul r6.xzw, r4.xxyz, r6.xxxx
      mad r6.xzw, r3.xxyz, r5.wwww, r6.xxzw
      mad r6.xyz, r2.xyzx, r6.yyyy, r6.xzwx
      dp3 r5.w, r2.xyzx, r6.xyzx
      add r5.w, r5.w, r5.w
      mad r6.xyz, r5.wwww, r6.xyzx, -r2.xyzx
      dp3_sat r5.w, r2.xyzx, r6.xyzx
      lt r6.w, l(0.000000), r5.w
      if_nz r6.w
        sample_l_indexable(texturecube)(float,float,float,float) r6.xyz, r6.xyzx, t0.xyzw, s0, l(0.000000)
        mad r5.xyz, r6.xyzx, r5.wwww, r5.xyzx
        add r3.w, r3.w, r5.w
      endif 
      iadd r4.w, r4.w, l(1)
    endloop 
    div r0.xyz, r5.xyzx, r3.wwww
  endif 
  mov r1.zw, l(0,0,0,0)
  mov r0.w, l(1.000000)
  store_uav_typed u0.xyzw, r1.xyzw, r0.xyzw
endif 
ret 
// Approximately 153 instruction slots used
#endif

const BYTE cubemap_filter[] =
{
     68,  88,  66,  67,  60,  33, 
     22, 212,  83,  42, 172,  69, 
    120, 232,  18, 184, 152, 130, 
     48, 146,   1,   0,   0,   0, 
    104,  21,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    232,   2,   0,   0, 248,   2, 
      0,   0,   8,   3,   0,   0, 
    204,  20,   0,   0,  82,  68, 
     69,  70, 172,   2,   0,   0, 
      1,   0,   0,   0, 216,   0, 
      0,   0,   4,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  67,   0,   1,   0,   0, 
    120,   2,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    188,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 194,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   9,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 200,   0, 
      0,   0,   4,   0,   0,   0, 
      5,   0,   0,   0,   5,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
    206,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 115, 109,  73,  66, 
     76,   0, 116, 120,  73,  66, 
     76,   0, 116, 120,  79,  85, 
     84,   0,  67,  83,  66, 117, 
    102, 102, 101, 114,   0, 171, 
    206,   0,   0,   0,   6,   0, 
      0,   0, 240,   0,   0,   0, 
     96,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    224,   1,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0, 244,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  24,   2, 
      0,   0,  16,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 244,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  35,   2,   0,   0, 
     32,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
    244,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     46,   2,   0,   0,  48,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0, 244,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  58,   2, 
      0,   0,  64,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 244,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  68,   2,   0,   0, 
     80,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
     84,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    122,  68, 105, 114, 101,  99, 
    116, 105, 111, 110,   0, 102, 
    108, 111,  97, 116,  52,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 235,   1, 
      0,   0, 120,  68, 105, 114, 
    101,  99, 116, 105, 111, 110, 
      0, 121,  68, 105, 114, 101, 
     99, 116, 105, 111, 110,   0, 
    116, 101, 120, 101, 108,  79, 
    102, 102, 115, 101, 116,   0, 
    114, 111, 117, 103, 104, 110, 
    101, 115, 115,   0, 117,  97, 
    118,  83, 105, 122, 101,   0, 
    117, 105, 110, 116,  52,   0, 
    171, 171,   1,   0,  19,   0, 
      1,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  76,   2, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  54,  46,  51,  46,  57, 
     54,  48,  48,  46,  49,  54, 
     51,  56,  52,   0, 171, 171, 
     73,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  79,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  83,  72,  69,  88, 
    188,  17,   0,   0,  80,   0, 
      5,   0, 111,   4,   0,   0, 
    106,   8,   0,   1,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  88,  48,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
    156,  64,   0,   4,   0, 224, 
     17,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  95,   0, 
      0,   2,  50,  16,   2,   0, 
     95,   0,   0,   2,  50,  32, 
      2,   0, 104,   0,   0,   2, 
      9,   0,   0,   0, 155,   0, 
      0,   4,   8,   0,   0,   0, 
      8,   0,   0,   0,   1,   0, 
      0,   0,  61,  16,   0, 137, 
      2,   2,   0, 128,  67,  85, 
     21,   0,  50,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     70, 238,  17,   0,   0,   0, 
      0,   0,  35,   0,   0,  10, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  70,  16,   2,   0, 
      2,  64,   0,   0,   8,   0, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  32,   2,   0, 
     79,   0,   0,   7, 194,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   4,  16,   0,   1,   0, 
      0,   0,   6,   4,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  31,   0,   4,   3, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  32,   0,   0,  10, 
     50,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  31,   0, 
      4,   3,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   9,  18,   0,  16,   0, 
      0,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  50,   0, 
      0,   9,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128, 191,  49,   0, 
      0,   9,  34,   0,  16,   0, 
      0,   0,   0,   0,  42, 128, 
     32, 128, 129,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
    119, 190, 127,  63,  55,   0, 
      0,  15, 226,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      2,   0,   0,   0, 150,   7, 
     16,   0,   0,   0,   0,   0, 
    150, 132,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 226,   0, 
     16,   0,   0,   0,   0,   0, 
    246,   9,  16,   0,   0,   0, 
      0,   0, 166, 132,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   6,   9,  16, 128, 
     65,   0,   0,   0,   2,   0, 
      0,   0,  16,   0,   0,   7, 
     18,   0,  16,   0,   2,   0, 
      0,   0, 150,   7,  16,   0, 
      0,   0,   0,   0, 150,   7, 
     16,   0,   0,   0,   0,   0, 
     68,   0,   0,   5,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   7, 
    226,   0,  16,   0,   0,   0, 
      0,   0,  86,  14,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     56,   0,   0,   8, 114,   0, 
     16,   0,   2,   0,   0,   0, 
    230,   9,  16,   0,   0,   0, 
      0,   0,  38, 137,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 150, 132,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 118,  14,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   8, 242,   0, 
     16,   0,   3,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  48,   0,   0,   1, 
     80,   0,   0,   7,  18,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   1,   0,   3,   0, 
      4,   3,  10,   0,  16,   0, 
      4,   0,   0,   0,  86,   0, 
      0,   5,  18,   0,  16,   0, 
      4,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
     85,   0,   0,   7,  34,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
     16,   0,   0,   0, 140,   0, 
      0,  11,  66,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,  16,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
     41,   0,   0,   7,  34,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0, 140,   0, 
      0,  11,  34,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,  16,   0,   0,   0, 
      1,  64,   0,   0,  17,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
     85,   0,   0,   7,  66,   0, 
     16,   0,   4,   0,   0,   0, 
     42,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,  10,  98,   0,  16,   0, 
      4,   0,   0,   0,  86,   6, 
     16,   0,   4,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0, 170, 170, 170, 170, 
     85,  85,  85,  85,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     34,   0,  16,   0,   4,   0, 
      0,   0,  42,   0,  16,   0, 
      4,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
     41,   0,   0,   7,  66,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0,  85,   0, 
      0,   7,  34,   0,  16,   0, 
      4,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,  10, 
     98,   0,  16,   0,   4,   0, 
      0,   0,  86,   6,  16,   0, 
      4,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
     51,  51,  51,  51, 204, 204, 
    204, 204,   0,   0,   0,   0, 
     30,   0,   0,   7,  34,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,  42,   0,  16,   0, 
      4,   0,   0,   0,  41,   0, 
      0,   7,  66,   0,  16,   0, 
      4,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   4,   0, 
      0,   0,  85,   0,   0,   7, 
     34,   0,  16,   0,   4,   0, 
      0,   0,  26,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   4,   0,   0,   0, 
      1,   0,   0,  10,  98,   0, 
     16,   0,   4,   0,   0,   0, 
     86,   6,  16,   0,   4,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,  15,  15, 
     15,  15, 240, 240, 240, 240, 
      0,   0,   0,   0,  30,   0, 
      0,   7,  34,   0,  16,   0, 
      4,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
     42,   0,  16,   0,   4,   0, 
      0,   0,  41,   0,   0,   7, 
     66,   0,  16,   0,   4,   0, 
      0,   0,  26,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   8,   0,   0,   0, 
     85,   0,   0,   7,  34,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      8,   0,   0,   0,   1,   0, 
      0,  10,  98,   0,  16,   0, 
      4,   0,   0,   0,  86,   6, 
     16,   0,   4,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0, 255,   0, 255,   0, 
      0, 255,   0, 255,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     34,   0,  16,   0,   4,   0, 
      0,   0,  26,   0,  16,   0, 
      4,   0,   0,   0,  42,   0, 
     16,   0,   4,   0,   0,   0, 
     86,   0,   0,   5,  34,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,  56,   0,   0,  10, 
     82,   0,  16,   0,   4,   0, 
      0,   0,   6,   1,  16,   0, 
      4,   0,   0,   0,   2,  64, 
      0,   0, 219,  15, 201,  56, 
      0,   0,   0,   0,   0,   0, 
    128,  47,   0,   0,   0,   0, 
     50,   0,   0,  10,  34,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  47,   1,  64,   0,   0, 
      0,   0, 128,  63,  50,   0, 
      0,   9,  66,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  14,   0, 
      0,   7,  34,   0,  16,   0, 
      4,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
     42,   0,  16,   0,   4,   0, 
      0,   0,  75,   0,   0,   5, 
     66,   0,  16,   0,   4,   0, 
      0,   0,  26,   0,  16,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   8,  34,   0,  16,   0, 
      4,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     75,   0,   0,   5,  34,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,  77,   0,   0,   7, 
     18,   0,  16,   0,   4,   0, 
      0,   0,  18,   0,  16,   0, 
      5,   0,   0,   0,  10,   0, 
     16,   0,   4,   0,   0,   0, 
     56,   0,   0,   7, 130,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,  10,   0,  16,   0, 
      5,   0,   0,   0,  56,   0, 
      0,   7,  18,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   4,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   5,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   4,   0,   0,   0, 
     50,   0,   0,   9, 178,   0, 
     16,   0,   4,   0,   0,   0, 
    150,  13,  16,   0,   0,   0, 
      0,   0, 246,  15,  16,   0, 
      4,   0,   0,   0,  70,   8, 
     16,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   4,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   4,   0, 
      0,   0,  70,   3,  16,   0, 
      4,   0,   0,   0,  16,   0, 
      0,   8, 130,   0,  16,   0, 
      4,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   4,   0,   0,   0, 
      0,   0,   0,   7, 130,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      4,   0,   0,   0,  50,   0, 
      0,  11, 114,   0,  16,   0, 
      4,   0,   0,   0, 246,  15, 
     16,   0,   4,   0,   0,   0, 
     70,   2,  16,   0,   4,   0, 
      0,   0,  70, 130,  32, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     16,  32,   0,   8, 130,   0, 
     16,   0,   4,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   4,   0, 
      0,   0,  49,   0,   0,   7, 
     18,   0,  16,   0,   5,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   4,   0,   0,   0, 
     31,   0,   4,   3,  10,   0, 
     16,   0,   5,   0,   0,   0, 
     72,   0,   0, 141, 130,   1, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      4,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   4,   0,   0,   0, 
    246,  15,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   7, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,  21,   0,   0,   1, 
     30,   0,   0,   7, 130,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,  22,   0, 
      0,   1,  14,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0, 246,  15, 
     16,   0,   2,   0,   0,   0, 
     18,   0,   0,   1,  30,   0, 
      0,   9,  50,   0,  16,   0, 
      2,   0,   0,   0,  70,   0, 
     16,   0,   1,   0,   0,   0, 
    230, 138,  32, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  43,   0, 
      0,   5,  50,   0,  16,   0, 
      2,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
     43,   0,   0,   6, 194,   0, 
     16,   0,   2,   0,   0,   0, 
    166, 142,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     14,   0,   0,   7,  50,   0, 
     16,   0,   2,   0,   0,   0, 
     70,   0,  16,   0,   2,   0, 
      0,   0, 230,  10,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   8,  50,   0,  16,   0, 
      2,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 128,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     50,   0,   0,  11, 210,   0, 
     16,   0,   2,   0,   0,   0, 
      6, 137,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   2,   0, 
      0,   0,   6, 137,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      2,   0,   0,   0, 134,   3, 
     16,   0,   2,   0,   0,   0, 
     16,   0,   0,   7, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  68,   0, 
      0,   5, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   2,   0,   0,   0, 
    246,  15,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   9, 130,   0,  16,   0, 
      2,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128, 191,  49,   0, 
      0,   8,  18,   0,  16,   0, 
      3,   0,   0,   0,  42,   0, 
     16, 128, 129,   0,   0,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0, 119, 190, 127,  63, 
     55,   0,   0,  15, 114,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   0,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   4,   0,   0,   0, 
    150,   4,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      3,   0,   0,   0,  38,   9, 
     16,   0,   3,   0,   0,   0, 
     38,   9,  16,   0,   2,   0, 
      0,   0,  70,   2,  16, 128, 
     65,   0,   0,   0,   4,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   3,   0, 
      0,   0, 246,  15,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   4,   0,   0,   0, 
     38,   9,  16,   0,   2,   0, 
      0,   0, 150,   4,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      4,   0,   0,   0, 150,   4, 
     16,   0,   2,   0,   0,   0, 
     38,   9,  16,   0,   3,   0, 
      0,   0,  70,   2,  16, 128, 
     65,   0,   0,   0,   4,   0, 
      0,   0,  54,   0,   0,   8, 
    114,   0,  16,   0,   5,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  48,   0,   0,   1, 
     80,   0,   0,   7, 130,   0, 
     16,   0,   5,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   1,   0,   3,   0, 
      4,   3,  58,   0,  16,   0, 
      5,   0,   0,   0,  86,   0, 
      0,   5, 130,   0,  16,   0, 
      5,   0,   0,   0,  58,   0, 
     16,   0,   4,   0,   0,   0, 
     85,   0,   0,   7,  18,   0, 
     16,   0,   6,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
     16,   0,   0,   0, 140,   0, 
      0,  11,  34,   0,  16,   0, 
      6,   0,   0,   0,   1,  64, 
      0,   0,  16,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  58,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     41,   0,   0,   7,  18,   0, 
     16,   0,   6,   0,   0,   0, 
     10,   0,  16,   0,   6,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0, 140,   0, 
      0,  11,  18,   0,  16,   0, 
      6,   0,   0,   0,   1,  64, 
      0,   0,  16,   0,   0,   0, 
      1,  64,   0,   0,  17,   0, 
      0,   0,  58,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     85,   0,   0,   7,  34,   0, 
     16,   0,   6,   0,   0,   0, 
     26,   0,  16,   0,   6,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,  10,  50,   0,  16,   0, 
      6,   0,   0,   0,  70,   0, 
     16,   0,   6,   0,   0,   0, 
      2,  64,   0,   0, 170, 170, 
    170, 170,  85,  85,  85,  85, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   6,   0, 
      0,   0,  26,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     41,   0,   0,   7,  34,   0, 
     16,   0,   6,   0,   0,   0, 
     10,   0,  16,   0,   6,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0,  85,   0, 
      0,   7,  18,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,  10, 
     50,   0,  16,   0,   6,   0, 
      0,   0,  70,   0,  16,   0, 
      6,   0,   0,   0,   2,  64, 
      0,   0,  51,  51,  51,  51, 
    204, 204, 204, 204,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     30,   0,   0,   7,  18,   0, 
     16,   0,   6,   0,   0,   0, 
     10,   0,  16,   0,   6,   0, 
      0,   0,  26,   0,  16,   0, 
      6,   0,   0,   0,  41,   0, 
      0,   7,  34,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
      1,  64,   0,   0,   4,   0, 
      0,   0,  85,   0,   0,   7, 
     18,   0,  16,   0,   6,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,   1,  64, 
      0,   0,   4,   0,   0,   0, 
      1,   0,   0,  10,  50,   0, 
     16,   0,   6,   0,   0,   0, 
     70,   0,  16,   0,   6,   0, 
      0,   0,   2,  64,   0,   0, 
     15,  15,  15,  15, 240, 240, 
    240, 240,   0,   0,   0,   0, 
      0,   0,   0,   0,  30,   0, 
      0,   7,  18,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     26,   0,  16,   0,   6,   0, 
      0,   0,  41,   0,   0,   7, 
     34,   0,  16,   0,   6,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,   1,  64, 
      0,   0,   8,   0,   0,   0, 
     85,   0,   0,   7,  18,   0, 
     16,   0,   6,   0,   0,   0, 
     10,   0,  16,   0,   6,   0, 
      0,   0,   1,  64,   0,   0, 
      8,   0,   0,   0,   1,   0, 
      0,  10,  50,   0,  16,   0, 
      6,   0,   0,   0,  70,   0, 
     16,   0,   6,   0,   0,   0, 
      2,  64,   0,   0, 255,   0, 
    255,   0,   0, 255,   0, 255, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   6,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,  26,   0, 
     16,   0,   6,   0,   0,   0, 
     86,   0,   0,   5,  18,   0, 
     16,   0,   6,   0,   0,   0, 
     10,   0,  16,   0,   6,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   6,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  47, 
     56,   0,   0,   7, 130,   0, 
     16,   0,   5,   0,   0,   0, 
     58,   0,  16,   0,   5,   0, 
      0,   0,   1,  64,   0,   0, 
    219,  15, 201,  56,  50,   0, 
      0,  10,  18,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16, 128,  65,   0,   0,   0, 
      6,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  47, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  50,   0,   0,   9, 
     34,   0,  16,   0,   6,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   6,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  14,   0,   0,   7, 
     18,   0,  16,   0,   6,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,  26,   0, 
     16,   0,   6,   0,   0,   0, 
     75,   0,   0,   5,  34,   0, 
     16,   0,   6,   0,   0,   0, 
     10,   0,  16,   0,   6,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   6,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   6,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  75,   0, 
      0,   5,  18,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     77,   0,   0,   7,  18,   0, 
     16,   0,   7,   0,   0,   0, 
     18,   0,  16,   0,   8,   0, 
      0,   0,  58,   0,  16,   0, 
      5,   0,   0,   0,  56,   0, 
      0,   7, 130,   0,  16,   0, 
      5,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     10,   0,  16,   0,   8,   0, 
      0,   0,  56,   0,   0,   7, 
     18,   0,  16,   0,   6,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16,   0,   7,   0,   0,   0, 
     56,   0,   0,   7, 210,   0, 
     16,   0,   6,   0,   0,   0, 
      6,   9,  16,   0,   4,   0, 
      0,   0,   6,   0,  16,   0, 
      6,   0,   0,   0,  50,   0, 
      0,   9, 210,   0,  16,   0, 
      6,   0,   0,   0,   6,   9, 
     16,   0,   3,   0,   0,   0, 
    246,  15,  16,   0,   5,   0, 
      0,   0,   6,  14,  16,   0, 
      6,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      6,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   6,   0, 
      0,   0, 134,   3,  16,   0, 
      6,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      5,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   6,   0, 
      0,   0,   0,   0,   0,   7, 
    130,   0,  16,   0,   5,   0, 
      0,   0,  58,   0,  16,   0, 
      5,   0,   0,   0,  58,   0, 
     16,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   6,   0,   0,   0, 
    246,  15,  16,   0,   5,   0, 
      0,   0,  70,   2,  16,   0, 
      6,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      2,   0,   0,   0,  16,  32, 
      0,   7, 130,   0,  16,   0, 
      5,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   6,   0, 
      0,   0,  49,   0,   0,   7, 
    130,   0,  16,   0,   6,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   5,   0,   0,   0, 
     31,   0,   4,   3,  58,   0, 
     16,   0,   6,   0,   0,   0, 
     72,   0,   0, 141, 130,   1, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   6,   0, 
      0,   0,  70,   2,  16,   0, 
      6,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      5,   0,   0,   0,  70,   2, 
     16,   0,   6,   0,   0,   0, 
    246,  15,  16,   0,   5,   0, 
      0,   0,  70,   2,  16,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   7, 130,   0,  16,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16,   0,   5,   0, 
      0,   0,  21,   0,   0,   1, 
     30,   0,   0,   7, 130,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,  22,   0, 
      0,   1,  14,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      5,   0,   0,   0, 246,  15, 
     16,   0,   3,   0,   0,   0, 
     21,   0,   0,   1,  54,   0, 
      0,   8, 194,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63, 164,   0,   0,   7, 
    242, 224,  17,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0, 153,   0, 
      0,   0,   9,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  68,   0,   0,   0, 
     21,   0,   0,   0,  23,   0, 
      0,   0,   2,   0,   0,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
      2,   0,   0,   0,   6,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0
};
