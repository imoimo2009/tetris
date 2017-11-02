///////////////////////////////////////////////////////////////////////////////

                        テトリス的なアレ。(SE○A仕様)

                       Programmed by 2010/07/22 いもいも

///////////////////////////////////////////////////////////////////////////////


●概要
  某有名ゲームに似てる気がしないでもないゲームです。
  実装実験的に作っていたらいつの間にかあんな感じやこんな感じに
  なっちゃったので、こっそり公開してみます。


●インストール＆起動方法
  zipファイルを適当な解凍ソフト(Lhaplus等)で解凍してください。
  指定したフォルダ上に「tetris」というフォルダが作成され、
  その中にゲームに必要なファイルが解凍されます。
  さらにその中にあるtetris.exeをダブルクリックすればゲームが起動します。


●ルール
  基本的にテ○リスの国際ルールに準拠してますが、
  一部セ○テト○スのルールも採用しています。


●操作方法
  ※方向キーはそれぞれゲームパッドの十字キーに対応しています。
  ↑...ハードドロップ(瞬間落下・オプションで有効時)
  ↓...高速落下(高速落下中の左右移動は出来ません)
  →...右移動
  ←...左移動
  ※Z,X,Cキーについてはオプション画面でキー割り当てを変更出来ます。
  Ｚ(PAD1ボタン)...左回転・決定
  Ｘ(PAD2ボタン)...右回転・キャンセル
  Ｃ(PAD3ボタン)...ホールド(オプションで有効時)
  [ESC]...ゲーム終了


●著作権・ライセンス表記

  ・このプログラムを用いたことによって生じた如何なる損害に対しても
    著作権保有者はその保障義務を一切負わないものとします。

  ・著作権保有者はこのプログラムに不備が有っても、
    それを訂正する義務を負いません。

  ・添付したソースファイルについては、使用者の責任において
    自由に改変・流用してもかまいません。
    その場合は、その旨を記載する必要もありません。

  ・このプログラムは「ＤＸライブラリ」を使用して制作しております。
    DX Library Copyright (C) 2001-2010 Takumi Yamada.

  ・JPEG 画像を読みこむ機能

	libjpeg　Copyright (C) 1991-1998, Thomas G. Lane.
	this software is based in part on the work of the 
	Independent JPEG Group.

  ・Ogg Vorbis 音声データを読み込む機能

	ogg_static, vorbis_static, vorbisfile_static
	Copyright (C) 1994-2002 XIPHOPHORUS Company http://www.xiph.org/ .

	Please note that this is only a sample license; authoritative license
	terms are included with each software bundle. 
	　　　　　　
	c year, Xiph.Org Foundation
	　　　　　　
	Redistribution and use in source and binary forms,
	with or without modification,
	are permitted provided that the following conditions are met: 
	　　　　　　
	- Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer. 

	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution. 

	- Neither the name of the Xiph.org Foundation nor the names of its
	  contributors may be used to endorse or promote products derived from
	  this software without specific prior written permission. 

	This software is provided by the copyright holders and contributors
	“as is” and any express or implied warranties, including, but not 
	limited to, the implied warranties　of merchantability and fitness for
　　　　a particular purpose are disclaimed. In no event shall　the foundation
	or contributors be liable for any direct, indirect, incidental,
	special, exemplary, or consequential damages
	(including, but not limited to, procurement of substitute goods or 
         services; loss of use, data, or profits; or business interruption)
	however caused and on any theory of liability, whether in contract,
	strict liability, or tort(including negligence or otherwise)
	arising in any way out of the use of this software, even if advised of
	the possibility of such damage. 

  ・関数 GetRand

	Mersenne Twister
	Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.

	3. The name of the author may not be used to endorse or promote
	   products derived from this software without specific prior written
	   permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
        DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
        (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
         SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
        HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
        STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
        ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
        THE POSSIBILITY OF SUCH DAMAGE.


●更新履歴

  Ver.1.00 公開バージョン。
