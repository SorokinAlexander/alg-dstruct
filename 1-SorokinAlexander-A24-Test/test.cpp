#include "pch.h"
#include "Functions.h"
#include "gtest/gtest.h"


TEST(UnionStr, UnionStringsTest) {
	char* str1 = "Hello,";
	char* str2 = "World!";
	EXPECT_STREQ(UnionStr(str1, str2), "Hello,World!");
}

TEST(ReadFromFile, EmptyFileTest) {
	int linesNum = 0;
	char** lines = ReadFromFile(&linesNum, "C:\\repos\\alg-dstruct\\1-SorokinAlexander-A24\\list_empty.txt");
	EXPECT_EQ(linesNum, 0);
	EXPECT_TRUE(lines == nullptr);
}

TEST(ReadFromFile, FilledFileTest) {
	int linesNum = 0;
	char* line1 = "15.10.2021 Alexander Sorokin 57\n";
	char* line2 = "16.10.2021 Lev Mukhachev 24\n";
	char* line3 = "18.10.2021 Pavel Popov 32\n";
	char* line4 = "11.10.2021 Nikolay Sobolev 32\n";
	char* line5 = "10.10.2021 Zendaya Coleman 87\n";
	/*FILE* tmp = fopen("C:\\repos\\alg-dstruct\\1-SorokinAlexander-A24\\labor.txt", "w");
	fprintf(tmp, "%s", line1);
	fprintf(tmp, "%s", line2);
	fprintf(tmp, "%s", line3);
	fprintf(tmp, "%s", line4);
	fprintf(tmp, "%s", line5);
	fclose(tmp);
	*/
	char** lines = ReadFromFile(&linesNum, "C:\\repos\\alg-dstruct\\1-SorokinAlexander-A24\\list.txt");

	EXPECT_EQ(linesNum, 5);

	EXPECT_STREQ(lines[0], line1);
	EXPECT_STREQ(lines[1], line2);
	EXPECT_STREQ(lines[2], line3);
	EXPECT_STREQ(lines[3], line4);
	EXPECT_STREQ(lines[4], line5);
}

TEST(NodeCreate, EmptyLineTest) {
	char* line = NULL;
	struct worker_t* p = NodeCreate(line);
	EXPECT_TRUE(p == nullptr);
}

TEST(NodeCreate, FilledLineTest) {
	char* line = "15.10.2021 Alexander Sorokin 57\n";
	struct worker_t* p = NodeCreate(line);

	EXPECT_STREQ(p->date, "15.10.2021");
	EXPECT_STREQ(p->name, "Alexander");
	EXPECT_STREQ(p->surname, "Sorokin");
	EXPECT_EQ(p->workHours, 57);
}

TEST(AddNodeAfter, AddAfterTest) {
	char* line1 = "15.10.2021 Alexander Sorokin 57\n";
	char* line2 = "16.10.2021 Lev Mukhachev 24\n";
	struct worker_t* pHead = NodeCreate(line1);
	struct worker_t* pNode = NodeCreate(line2);
	AddNodeAfter(pHead, pNode);
	EXPECT_TRUE(pHead->next == pNode);
	EXPECT_TRUE(pNode->next == NULL);
	EXPECT_STREQ(pHead->name, "Alexander");
	EXPECT_STREQ(pNode->name, "Lev");
}

TEST(AddNodeBefore, AddBeforeTest) {
	char* line1 = "15.10.2021 Alexander Sorokin 57\n";
	char* line2 = "16.10.2021 Lev Mukhachev 24\n";
	struct worker_t* pHead = NodeCreate(line1);
	struct worker_t* pNode = NodeCreate(line2);
	AddNodeBefore(pHead, pNode);
	EXPECT_TRUE(pHead->next == pNode);
	EXPECT_TRUE(pNode->next == NULL);
	EXPECT_STREQ(pHead->name, "Lev");
	EXPECT_STREQ(pNode->name, "Alexander");
}

TEST(ListCreate) {
	int linesNum = 3;
	char* lines[] = { "15.10.2021 Alexander Sorokin 57\n",
					"18.10.2021 Pavel Popov 32\n",
					"11.10.2021 Nikolay Sobolev 32\n" };
	struct worker_t* pHead = ListCreate(linesNum, lines);
	EXPECT_STREQ(pHead->name, "Nikolay");
	pHead = pHead->next;
	EXPECT_STREQ(pHead->name, "Pavel");
	pHead = pHead->next;
	EXPECT_STREQ(pHead->name, "Alexander");
	EXPECT_TRUE(pHead->next == NULL);
}

TEST(LineOutput, EmptyLineTest) {
	struct worker_t* p = NULL;
	char* line = LineOutput(p, p, 100);
	EXPECT_TRUE(line == nullptr);
}

TEST(LineOutput, NIsBiggerTest) {
	struct worker_t worker = { "18.10.2021", "Alexander", "Sorokin", 70, NULL };
	char* line = LineOutput(&worker, &worker, 100);
	EXPECT_TRUE(line == nullptr);
}

TEST(LineOutput, FilledTwoLinesTest) {
	struct worker_t worker2 = { "18.10.2021", "Alexander", "Sorokin", 70, NULL };
	struct worker_t worker1 = { "29.10.2020", "Alexander", "Sorokin", 80, &worker2 };
	char* line = LineOutput(&worker1, &worker2, 100);
	EXPECT_STREQ(line, "18.10.2021 Sorokin");
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	return 0;
}