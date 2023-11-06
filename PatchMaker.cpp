#include "PatchMaker.h"

// �������� ������ �� api, ��� �� git
void PatchMaker::loadSpecs(specLoader sl) {
	if (sl == specLoader::apiLoader) {
		SpecCollector s;
		for (int i = 0; i < packagesToPatch.size(); i++) {
			specs[packagesToPatch[i]] = s.getSpec(branch, packagesToPatch[i]);
		}
	}
	if (sl == specLoader::gitLoader) {
		// TODO: ������� �������� � ����
	}
}

std::string prepareDependencyType(std::string s) {
    std::transform(s.begin(), s.begin() + 1, s.begin(), ::toupper);
    std::transform(s.begin() + 1, s.end(), s.begin() + 1, ::tolower);
    return s + ":";
}


std::string generatePatch(std::string spec, std::vector<Dependency>& ds) {
    for (int i = 0; i < ds.size(); i++) {
        std::string deptype = prepareDependencyType(ds[i].type);
        std::string version = "( " + ds[i].sign + " " + ds[i].version + ")";
        std::string ex;

        if (ds[i].version == "") {
            version = "([^,\n]*)";
        }

        ex = "(" + prepareDependencyType(ds[i].type) + " .*)";
        ex += "(" + ds[i].dependencyName + ")";
        ex += version + "(, )";
        std::cout << ex << std::endl;
        std::regex expr(ex, std::regex::icase);
        spec = std::regex_replace(spec, expr, "$1");

        if (ds[i].version == "") {
            version = "([^\n])*";
        }

        ex = "(" + prepareDependencyType(ds[i].type) + " )";
        ex += "(" + ds[i].dependencyName + ")";
        ex += version + "(\\r|\\n|\\0)";
        std::cout << ex << std::endl;
        std::regex expr2(ex, std::regex::icase);
        spec = std::regex_replace(spec, expr2, "");

        ex = "(" + prepareDependencyType(ds[i].type) + " .*)";
        ex += "(, " + ds[i].dependencyName + ")";
        ex += version + "(\\r|\\n|\\0)";
        std::cout << ex << std::endl;
        std::regex expr3(ex, std::regex::icase);
        spec = std::regex_replace(spec, expr3, "$1\n");
    }
    return spec;
}

// ���������� ���� �� �������� ������ � ��������� ��� �� ���������� ����
void PatchMaker::makePatch(std::string patch_destination) {
	for (int i = 0; i < packagesToPatch.size(); i++) {
		FILE* spec = fopen("Specfile.spec", "wb");
		spec >> specs[packagesToPatch[i]];
		Fclose(spec);

        FILE* patched = fopen("PatchedSpecfile.spec", "wb");
        std::string patched_str = generatePatch(specs[packagesToPatch[i]], dependenciesToDelete[packagesToPatch[i]])
        patched >> patched_str;
        Fclose(spec);

        if (patched_str != specs[packagesToPatch[i]]) {
            int resp = system("diff -Naru Specfile.spec PatchedSpecfile.spec > " + patch_destination + packagesToPatch[i] + ".patch");
            if (resp != 0) {
                error++;
                std::cout << "Error while making patch" << resp << std::endl;
                return {};

            }
        }
	}
}