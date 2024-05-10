// Copyright kaunstdadenga


#include "AbilitySystem/Data/LevelUpInfo.h"

#include "Aura/AuraLogChannels.h"

int32 ULevelUpInfo::FindLevelForXP(const int32 XP) const {
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching) {
		// LevelInformation[0] = Level Placeholder
		// LevelInformation[1] = Level 1 Information
		// LevelInformation[2] = Level 2 Information
		if (LevelUpInformation.Num() - 1 <= Level) {
			return Level;
		}

		if (XP >= LevelUpInformation[Level].LevelUpRequirement) {
			Level++;
		}
		else {
			bSearching = false;
		}
	}

	return Level;
}
