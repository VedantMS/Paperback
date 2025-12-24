#include<filesystem>
#include "pdf_utils.h"

namespace fs = std::filesystem;

struct PdfStatus {
    std::string filename;
    bool added;
    int paper_id;
};

std::vector<std::string> list_pdfs(const std::string &folder)
{
    std::vector<std::string> result;

    if (!fs::exists(folder) || !fs::is_directory(folder))
        return result;

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() == ".pdf") {
            result.push_back(entry.path().filename().string());
        }
    }
    return result;
}

bool is_added(const std::vector<paper>& papers, const std::string& filename, int &out_id) {
    for (const auto &p : papers) {
        size_t pos = p.pdf_path.find_last_of("/\\");
        std::string stored = (pos == std::string::npos) ? p.pdf_path : p.pdf_path.substr(pos + 1);

        if (stored == filename) {
            out_id = p.id;
            return true;
        }
    }
    return false;
}

std::vector<PdfStatus>
get_pdf_status(const std::vector<paper>& Paper,
               const std::string& folder)
{
    std::vector<PdfStatus> result;
    std::vector<std::string> pdfs = list_pdfs(folder);

    for (const auto& pdf : pdfs) {
        int id = -1;
        bool added = is_added(Paper, pdf, id);

        result.push_back({
            pdf,
            added,
            added ? id : -1
        });
    }
    return result;
}
