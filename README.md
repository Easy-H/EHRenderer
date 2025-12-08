# EHRenderer

## 목표
- [rastertek.com DirectX11](https://rastertek.com/tutdx11win10.html) 강의 실습
    - 4강 적용 완료
- Modern C++ 스타일 적용
    - 스마트 포인터 사용
        - DirectX Com 객체: ComPtr
        - 그외 기타 객체: 표준 스마트 포인터(std::unique_ptr, std::shared_ptr 등)
    - 지역 변수 선언 시점 조정
        - 함수 시작부분에 선언 => 사용 직전에 선언
- CMake를 이용한 빌드 환경 구성

## 학습 노트(Notion)
- [Rastertek DirextX11](https://easy-h.notion.site/DirextX11-2c05e129e5ff8093b435de39b3eab01a)
    - [WIN32 GUI API](https://easy-h.notion.site/WIN32-GUI-API-2c05e129e5ff800fa775c5e0e0a80daf)