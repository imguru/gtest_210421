
#include <gmock/gmock.h>

struct Element {};

class Calc {
	int x;
public:
	virtual ~Calc() {}

	// 행위 기반 검증 대상이 아닙니다.
	virtual int Add(Element x) {
		printf("Add(Element)\n");
	}

	// 행위 기반 검증 대상
	virtual int Add(int times, Element x) {}
};

class User {
public:
	void Add(Calc* p) {
		p->Add(210, Element{});
		p->Add(10, Element{});
	}
};
//------------------
// 의존하는 객체가 상속 가능한 클래스라면, Google Mock을 통해 행위기반 검증을 수행하 수 있습니다.
//  => 테스트 전용 하위 클래스
class MockCalc : public Calc {
public:
	// 문제: 동일한 이름을 가지는 부모의 함수가 가려지는 현상이 발생합니다.
	// 해결방법:
	using Calc::Add; // !!!

	MOCK_METHOD(int, Add, (int times, Element x), (override));
};

// 인자 일치하는지 검증하기 위해서 필요합니다.
bool operator==(const Element& lhs, const Element& rhs) {
	return true;
}

TEST(UserTest, Add) {
	User user;
	MockCalc calc;

	// EXPECT_CALL(calc, Add).Times(2);
	EXPECT_CALL(calc, Add(10, Element{}));
	EXPECT_CALL(calc, Add(210, Element{}));
	calc.Add(Element{});
	
	user.Add(&calc);
}
