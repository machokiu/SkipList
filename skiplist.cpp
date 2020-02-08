//
// Created by Yusuf Pisan on 4/26/18.
// Modified by Marci Ma on 11/13/19
//

#include <climits>
#include <cstdlib>
#include <iostream>

#include "skiplist.h"

using namespace std;

ostream &operator<<(ostream &Out, const SkipList &SkipL) {
	for (int I = SkipL.Depth - 1; I >= 0; --I) {
		SkipList::SNode* Curr = SkipL.FrontGuards[I];
		Out << "Level: " << I << " -- ";
		while (Curr != nullptr) {
			Out << Curr->Data << ", ";
			Curr = Curr->Next;
		}
		Out << "\n";
	}
	return Out;
}

SkipList::SNode::SNode(int Data) : Data{ Data }, Next{ nullptr }, Prev{ nullptr },
UpLevel{ nullptr }, DownLevel{ nullptr } {
}

SkipList::SkipList(int Depth) : Depth{ Depth }{
	FrontGuards = new SNode* [Depth];
	RearGuards = new SNode* [Depth];
	for(int I = 0; I < Depth; ++I) {
		SNode* FGuard = new SNode(INT_MIN);
		SNode* RGuard = new SNode(INT_MAX);
		FrontGuards[I] = FGuard;
		RearGuards[I] = RGuard;
		FGuard->Next = RGuard;
		RGuard->Prev = FGuard;
		if (Depth > 1 && I > 0) {
			FrontGuards[I]->DownLevel = FrontGuards[I - 1];
			RearGuards[I]->DownLevel = RearGuards[I - 1];
			if (I < (Depth - 1)) {
				FrontGuards[I - 1]->UpLevel = FrontGuards[I];
				RearGuards[I - 1]->UpLevel = RearGuards[I];
			}
		}
	}
}


bool SkipList::alsoHigher() const {
	if (Depth > 1) {
		int Coin = rand() % 2;
		return Coin == 1 ;
	}
	return false;
}


bool SkipList::add(int Data) {
	int CurrLevel = 0;
	
	SNode* Curr = FrontGuards[0];
		if (Curr == RearGuards[0]) {
			Curr = new SNode(Data);
			Curr->Next = RearGuards[0];
			Curr->Prev = FrontGuards[0];
			FrontGuards[0]->Next = Curr;
			RearGuards[0]->Prev = Curr;
		}
		while (Curr != nullptr && (Curr->Data) < Data) {
			Curr = Curr->Next;
			if (Curr->Data == Data)
				return false;
		}
	SNode* Tmp = new SNode(Data);
	addBefore(Tmp, Curr);
	
	bool Coin = alsoHigher();
		while (Coin && CurrLevel < (Depth-1)) {
			CurrLevel++;
			SNode* NewUp = new SNode(Data);
			Tmp->UpLevel = NewUp;
			NewUp->DownLevel = Tmp;
			Curr = FrontGuards[CurrLevel];
			while (Curr != nullptr && (Curr->Data) < Data) {
				Curr = Curr->Next;
			}
			addBefore(NewUp, Curr);
			Tmp = NewUp;
			Coin = alsoHigher();
		}
  return true;
}

void SkipList::addBefore(SNode *NewNode, SNode *NextNode) {
	NewNode->Prev = NextNode->Prev;
	NextNode->Prev = NewNode;
	NewNode->Next = NextNode;
	if (NewNode->Prev != nullptr)
		NewNode->Prev->Next = NewNode;
}

SkipList::~SkipList() {
	clear();
}

void SkipList::clear() {
	for (int I = 0; I < Depth-1; ++I) {
		SNode* Curr = FrontGuards[I];
		while (Curr != nullptr) {
			SNode* Tmp = Curr;
			Curr = Curr->Next;
			delete Tmp;
		}
	}
	delete []FrontGuards;
	delete[]RearGuards;
}

bool SkipList::remove(int Data) {
	SNode* Curr = FrontGuards[Depth - 1];
	while (Curr != nullptr) {
		while (Curr->Next->Data <= Data) {
			Curr = Curr->Next;
			if (Curr->Data == Data) {
				Curr->Next->Prev = Curr->Prev;
				Curr->Prev->Next = Curr->Next;
			}
		}
		Curr = Curr->DownLevel;
	}
	return false; 
}


bool SkipList::contains(int Data) {
	SNode* Curr = FrontGuards[Depth -1];
	while (Curr != nullptr) {
		while (Curr->Next->Data <= Data) {
			Curr = Curr->Next;
			if (Curr->Data == Data) {
				return true;
			}
		}
		Curr = Curr->DownLevel;		
	}

	 if (Curr == nullptr)
		 return false;

  return false;
}
