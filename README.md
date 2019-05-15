# Miner Bob

2019년 4학년 1학기 게임인공지능 과목에서 제작한 프로젝트로  
FSM 형식으로 구현한 NPC AI 입니다
<br /><br />
Mat Buckland의 'Programming Game AI by Example' 책의 예제 중 하나입니다  
이 프로젝트는 책의 원본 소스코드를 수정하는 식으로 만들었습니다
<br /><br />
NPC의 FSM에 사용되는 State들의 정의를 Lua 스크립트로 작성하였고  
Lua Bind를 활용하여 C++ 코드와의 연동을 하였습니다  
전체적인 FSM 뼈대는 C++로 작성되었습니다
<br /><br />
NPC에는 광부 Bob과 그의 아내인 Elsa가 있습니다
Bob의 State로는 금광에서 금캐기, 술집에서 축내기, 은행에 저금하기, 집에서 휴식하기 및 밥먹기  
등이 있고, Bob의 Condition에 따라 한 State에서 다른 State로 전이됩니다
<br /><br />
Elsa는 항상 집에 거주하며 집안일을 합니다  
가끔 화장실을 가기도 하며, 남편인 Bob이 집에 오면 밥을 차려줍니다
<br /><br />
NPC간의 통신에는 메세지 시스템이 존재하며,  
메세지를 받은 NPC는 상황에 따라 메세지에 대응합니다
<br /><br /><br />
**데모영상**은 여기서 보실 수 있습니다  
https://youtu.be/lZxu_ms2pjA
