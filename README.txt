* ExtRgss �ɂ���
ExtRgss �́ARPG�c�N�[���̃Q�[���G���W�������������邱�Ƃ�ړI�Ƃ����v���W�F�N�g�ł��B
���݁ARPG�c�N�[��VX Ace�̂����ꕔ�̋@�\�݂̂�ΏۂɊJ�����Ă��܂��B
����ADirectX �ł̕`��e�X�g���s�������ł��B

�uRPG�c�N�[���v�́A������ЃA�X�L�[�A����ъ�����ЃG���^�[�u���C���̓o�^���W�ł��B

* �g�p���@

 1. msvcrt-ruby191.dll �� ExtRgss.so �� Game.exe �Ɠ����f�B���N�g���ɒu���Ă��������B

    * ���� *
      msvcrt-ruby191.dll �͓����̂��́A�܂��� LoadSo �̂��̂��g���Ă��������B
      ruby �ɓ�������Ă�����͖̂��O�͓����ł����܂������ʕ��ł��B�����ӂ��������B
      �悭�킩��Ȃ��l�́A������ msvcrt-ruby191.dll �͎g���Ȃ��A�Ƃ����o���Ă����Ă��������B

 3. �c�N�[���̃X�N���v�g�G�f�B�^�擪���珇�ɁAload_so.rb, ext_rgss.rb �𒣂�t���Ă��������B
    �܂��� require �� load �œǂݍ��ނ悤�ɂ��Ă��\���܂���B
    �X�N���v�g�̏��Ԃ́ALoadSo, ExtRgss, ���̑��̃X�N���v�g�i�u�� ���W���[���v��uVocab�v�j
    �ƂȂ�悤�ɂ��Ă��������B

* �r���h���@
ext_rgss.so �̃r���h����n�߂�ꍇ�́A

 + Makefile ���ǂ߂� make�iGNU Make �œ���m�F���Ă��܂��j
 + Windows DLL ���o�͉\�� C �R���p�C���iMinGW-w64 �œ���m�F���Ă��܂��j
 + d3dx9.lib�iDirectX SDK���ɂ���܂��B�\�[�X�R�[�h�Ɠ����ꏊ�ɒu���Ă��������j
 + libloadso.a, rgss.h�iLoadSo �̂��́j

���K�v�ɂȂ�܂��B�p�ӂł�����\�[�X�R�[�h�̂���f�B���N�g����
make LOADSO_DIR=�iLoadSo �̃f�B���N�g���j�Ǝ��s���邾���� dll ���o���オ��Ǝv���܂��B

* ���C�Z���X
LICENSE.txt �܂��� LICENSE.ja.txt �����ꂩ�̃t�@�C���ɏ]���Ă��������B
�ǂ������{�I�ɓ������̂ł����A�p����ǂނ̂����ȕ��̂��߂̘a�� .ja �̕��ł��B
��ɕs���������ĈႤ���̂ɂȂ��Ă���\��������܂��B
���̏ꍇ�A�����Ă���������Ə�����܂��B
