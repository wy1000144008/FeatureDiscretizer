# Feature Discretizerの開発
##目的
Feature Discretizerは最適決定木アルゴリズムODTのための特徴離散化ツールである．

ODTは二値の特徴のみ扱えるので，連続値を含む特徴を使用する前に，連続値特徴の離散化処理が必要である．

連続値特徴の離散化処理によって，ODTの性能が変わる．一般的に，

   1. 特徴を細かく離散化するほど，分類精度がよくなる傾向があるが，実行時間もそれに応じて増加してしまう．

   2. 特徴を大雑把に離散化する場合，実行時間は増えないが，精度が落ちる．そして，一部のデータセットにおいて，データのConsistency(一貫性，不変性)がなくなってしまう．


本ツールの目標は，上の考察に基づいて，具体的なデータセットに対して，適切な離散化を行うことである．

本ツールが実現するのは，

  1. データのConsistencyを維持するまま，できる限りに大まかに特徴の離散化を行う．
  2.  1.のデータセットがODTに使える最低限のもの．ODTアルゴリズム精度の向上のため，1.に基づいて，特徴を細かく離散化を行う．

## API
### 言語
C++
###入力

1. 離散化したいデータセット
2. データセットの記述
	1. 特徴，ターゲット変数，dummy_headなど
	2. データセット中に離散化したい特徴など
3. 離散化ストラテジー (quantile, k-meansなど)

###出力
1. 離散化したデータセット
2. 離散化したデータセットの説明 (どの特徴について，どのように離散化したかなどの情報)

