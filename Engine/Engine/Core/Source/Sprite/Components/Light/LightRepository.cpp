#include "LightRepository.h"

#include "Sprite/Sprite.h"
#include "Camera/Camera.h"

namespace spe
{
    // Ctor
    LightRepository::LightRepository()
    {
        this->m_Index = 0;
        this->m_Update = false;
    }

    // Public

    void LightRepository::Init(const std::string& shader)
    {
        this->m_Update = true;
        this->m_Index = 0;
        if (!m_LightShader.loadFromFile(shader, sf::Shader::Fragment))
        {
            spe::Log::LogString("Could not load light shader");
        }
    }


    void LightRepository::UpdateLightSource(spe::Sprite* sprite, spe::Camera* cam)
    {
        if (!sprite->Light.Exist || !sprite->Process)
        {
            return;
        }

        m_Update = true;

        uint32_t idx = sprite->Light.GetLightIndex();
        spe::LightSource& source = m_LightSources[idx];

        if (sprite->Transform.PositionChanged || cam->HasZoomChanged())
        {
            sprite->Transform.PositionChanged = false;

            float zoom = cam->GetZoom() - 1;

            const float a = ((sprite->Transform.GetPosition().Y * -1) + 540) + 540 * zoom;
            spe::Vector2 new_pos = spe::Vector2((sprite->Transform.GetPosition().X + 960) + 960 * zoom, a);

            source.Position = new_pos;
        }
        if (sprite->Light.HasRadiusChanged())
        {
            source.Radius = sprite->Light.GetRadius();
        }
        if (sprite->Light.HasIntensityChanged())
        {
            source.LightIntensity = sprite->Light.GetIntensity();

        }
        if (sprite->Light.HasColorChanged())
        {
            source.Color = sprite->Light.GetColor();
        }

        if (m_Update)
        {
            sprite->Light.DisableFlags();
        }
    }

    void LightRepository::UpdateSprite(spe::Sprite* sprite, spe::Camera* cam)
    {
        if (!sprite->Light.Exist)
        {
            return;
        }
        UpdateLightSource(sprite, cam);
        UpdateArrays();
    }

    void LightRepository::Add(const spe::Vector2& pos, float radius, float intensity, const sf::Vector3f& color)
    {
        float zoom = 1;

        const float a = ((pos.Y * -1) + 540) + 540 * zoom;
        spe::Vector2 new_pos = spe::Vector2((pos.X + 960) + 960 * zoom, a);

        m_Index++;
        m_LightSources[m_Index] = spe::LightSource(new_pos, radius, intensity, color);

        m_Update = true;
        UpdateArrays();
    }

    void LightRepository::Remove(uint32_t index)
    {
        m_LightSources.erase(index);

        m_Update = true;
        UpdateArrays();
    }

    void LightRepository::MoveLightSource(uint32_t idx, const spe::Vector2& pos)
    {
        auto it = m_LightSources.find(idx);

        if (it != m_LightSources.end())
        {
            it->second.Position = pos;

            sf::Vector2f* lightPositions = GetPositionArray();

            m_LightShader.setUniformArray("lightPositions", lightPositions, m_LightSources.size());

            delete[] lightPositions;
        }
        else
        {
            std::cout << "LOG [ERROR] Key does not exist " << idx << std::endl;
        }
    }

    void LightRepository::UpdateArrays()
    {
        if (!this->m_Update)
        {
            return;
        }

        m_Update = false;
        size_t size = 0;

        std::vector<sf::Vector2f> lightPositions(size);
        std::vector<float> lightRadii(size);
        std::vector<float> lightIntensities(size);
        std::vector<sf::Vector3f> lightColors(size);


        for (auto& pair : m_LightSources)
        {
            LightSource& source = pair.second;
            if (source.Process)
            {
                lightPositions.push_back(spe::Vector2::toSFVector(source.Position));
                lightRadii.push_back(source.Radius);
                lightIntensities.push_back(source.LightIntensity);
                lightColors.push_back(sf::Vector3f(source.Color.x, source.Color.y, source.Color.z));
                size++;
            }
        }

        if (size > 0)
        {
            std::cout << "seting u " << size << std::endl;
            m_LightShader.setUniform("lightAmount", static_cast<int>(size));
            m_LightShader.setUniformArray("lightPositions", lightPositions.data(), size);
            m_LightShader.setUniformArray("lightRadii", lightRadii.data(), size);
            m_LightShader.setUniformArray("lightIntensities", lightIntensities.data(), size);
            m_LightShader.setUniformArray("lightColors", lightColors.data(), size);
        }
  
    }

    sf::Vector2f* LightRepository::GetPositionArray()
    {
        const size_t size = m_LightSources.size();
        sf::Vector2f* lightPositions = new sf::Vector2f[size];

        size_t i = 0;
        for (const auto& pair : m_LightSources)
        {
            lightPositions[i] = spe::Vector2::toSFVector(pair.second.Position);
            i++;
        }

        return lightPositions;
    }
}
